#include "renderer.hpp"
#include "pipeline.hpp"
#include "common.hpp"

namespace Celer {
	namespace Render {
		void Renderer::drawFrame(Core::VulkanContext &vulkanCtx, Core::SwapchainContext& swapchainCtx, Pipeline &pipeline, Core::Swapchain& swapchain, Core::Window& window, Geometry::MeshManager &meshManager, Core::DeviceMemoryManager& memManager, Core::FrameContext &frameCtx) {

			auto fenceResult{ vulkanCtx.device->waitForFences(*mFenceCollection[mCurrentFrameIdx], vk::True, UINT64_MAX) };

			if (fenceResult != vk::Result::eSuccess) {
				throw std::runtime_error("Failed to wait for fence");
			}

			auto [result, imageIndex] = swapchainCtx.swapchain->acquireNextImage(UINT64_MAX, mPresentFinished[mCurrentFrameIdx]/*Honestly has nothing to do with present being finished(well kind of) but better name this imageProcessingSlot instead*/);

			if (result == vk::Result::eErrorOutOfDateKHR) {
				swapchain.recreateSwapchain(vulkanCtx, window);
				return;
			} else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
				throw std::runtime_error("Failed to get swapchain image!");
			}


			mCommandBuffer.getCommandBuffer(mCurrentFrameIdx).reset();


			vulkanCtx.device->resetFences(*mFenceCollection[mCurrentFrameIdx]);

			//updateUniformBuffer(frameIdx);

			if (memManager.getMainBuffOwner() != vulkanCtx.graphicsQueueIdx) {
				memManager.mainBuffAcquireQueueOwnership(mCommandBuffer, mCurrentFrameIdx, memManager.getMainBuffOwner(), vulkanCtx.graphicsQueueIdx, *vulkanCtx.graphicsQueue, mTransferFence);
				vulkanCtx.device->waitForFences(*mTransferFence, vk::True, UINT64_MAX);
				vulkanCtx.device->resetFences(*mTransferFence);
				mCommandBuffer.getCommandBuffer(mCurrentFrameIdx).reset();
			}

			recordDrawCommands(imageIndex, swapchainCtx, pipeline, meshManager, window);



			frameCtx.mTimelineCount++;

			//std::cout << frameCtx.mFrameSyncObject.mSemaphore.getCounterValue() << " " << frameCtx.mTimelineCount << '\n';


			vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput); /*only blocks when the gpu needs to write pixels to the image*/
			
			vk::SemaphoreSubmitInfo waitInfo{
				.semaphore = *mPresentFinished[mCurrentFrameIdx],
				.stageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput
			};

			vk::CommandBufferSubmitInfo commandBuffInfo{
				.commandBuffer = *mCommandBuffer.getCommandBuffer(mCurrentFrameIdx)
			};

			vk::SemaphoreSubmitInfo binary {
				.semaphore = *mRenderFinished[imageIndex],
				.stageMask = vk::PipelineStageFlagBits2::eAllCommands
			};

			vk::SemaphoreSubmitInfo timeline{
				.semaphore = *frameCtx.mFrameSyncObject.mSemaphore,
				.value = frameCtx.mTimelineCount,
				.stageMask = vk::PipelineStageFlagBits2::eAllCommands
			};

			std::array<vk::SemaphoreSubmitInfo, 2> semaphoreInfos{ binary, timeline };
			//const vk::SubmitInfo submitInfo{ .waitSemaphoreCount = 1, .pWaitSemaphores = &*mPresentFinished[mCurrentFrameIdx], .pWaitDstStageMask = &waitDestinationStageMask, .commandBufferCount = 1, .pCommandBuffers = &*mCommandBuffer.getCommandBuffer(mCurrentFrameIdx), .signalSemaphoreCount = 1, .pSignalSemaphores = &*mRenderFinished[imageIndex] }; previous


			const vk::SubmitInfo2 submitInfo{
				.flags = {},
				.waitSemaphoreInfoCount = 1, .pWaitSemaphoreInfos = &waitInfo,
				.commandBufferInfoCount = 1, .pCommandBufferInfos = &commandBuffInfo,
				.signalSemaphoreInfoCount = 2, .pSignalSemaphoreInfos = semaphoreInfos.data()

			};


			//std::cout << "BEFORE SUBMIT timeline Count: " << frameCtx.mTimelineCount << " vs global: " << frameCtx.mFrameSyncObject.mSemaphore.getCounterValue() << '\n';


			vulkanCtx.graphicsQueue->submit2(submitInfo, *mFenceCollection[mCurrentFrameIdx]);
			
			//vulkanCtx.graphicsQueue->submit(submitInfo, );


			const vk::PresentInfoKHR presentInfoKHR{ .waitSemaphoreCount = 1, .pWaitSemaphores = &*mRenderFinished[imageIndex], .swapchainCount = 1, .pSwapchains = &(**swapchainCtx.swapchain), .pImageIndices = &imageIndex };
			result = vulkanCtx.graphicsQueue->presentKHR(presentInfoKHR);
			if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || window.getResized() ) {
				window.resized(false);
				swapchain.recreateSwapchain(vulkanCtx, window);
			}



			test[mCurrentFrameIdx]++;


			//std::cout << "AFTER SUBMIT timeline Count: " << frameCtx.mTimelineCount << " vs global: " << frameCtx.mFrameSyncObject.mSemaphore.getCounterValue() << '\n';
			//			
			mCurrentFrameIdx = (mCurrentFrameIdx + 1) % MAX_FRAMES_IN_FLIGHT;
		
		}
		void Renderer::recordDrawCommands(uint32_t imageIdx, Core::SwapchainContext& swapchainCtx, Pipeline& pipeline, Geometry::MeshManager& meshManager, Core::Window &window) {
			vk::raii::CommandBuffer& currentCommandBuff{ mCommandBuffer.getCommandBuffer(mCurrentFrameIdx) };

			auto& swapChainImg{ swapchainCtx.swapChainImages->getImage(imageIdx) };

			currentCommandBuff.begin({});

			/*
			imageIndex — which swapchain image you're transitioning
			eUndefined — old layout. "I don't care what it was before, throw away the contents"
			eColorAttachmentOptimal — new layout. "Rearrange it for drawing to"
			{} (srcAccessMask) — no access to wait on. Nothing was using this image before
			eColorAttachmentWrite — the next thing doing is writing color to it
			eColorAttachmentOutput (srcStage) — wait for the color output stage to be free
			eColorAttachmentOutput (dstStage) — the stage that will use it next is also color output
			*/
			swapChainImg.transitionImageLayout(vk::ImageLayout::eUndefined, vk::ImageLayout::eColorAttachmentOptimal, {}, vk::AccessFlagBits2::eColorAttachmentWrite, vk::PipelineStageFlagBits2::eColorAttachmentOutput, vk::PipelineStageFlagBits2::eColorAttachmentOutput, vk::ImageAspectFlagBits::eColor, currentCommandBuff);
			//transition_image_layout(
			//	*depthImage,
			//	vk::ImageLayout::eUndefined,
			//	vk::ImageLayout::eDepthAttachmentOptimal,
			//	vk::AccessFlagBits2::eDepthStencilAttachmentWrite,
			//	vk::AccessFlagBits2::eDepthStencilAttachmentWrite,
			//	vk::PipelineStageFlagBits2::eEarlyFragmentTests | vk::PipelineStageFlagBits2::eLateFragmentTests,
			//	vk::PipelineStageFlagBits2::eEarlyFragmentTests | vk::PipelineStageFlagBits2::eLateFragmentTests,
			//	vk::ImageAspectFlagBits::eDepth
			//);

			vk::ClearValue clearColor{ vk::ClearColorValue(0.f,0.f,0.f,1.f) };
			//vk::ClearDepthStencilValue depthClearValue{ .depth = 1.f, .stencil = 0 };

			/*imageView — which swapchain image view to render to
			imageLayout — the layout it's in (you just transitioned it to COLOR_ATTACHMENT_OPTIMAL)
			loadOp — what to do before rendering. Clear = wipe it to black first
			storeOp — what to do after rendering. Store = keep the pixels (you need them for presenting)
			clearValue — the black color (RGBA 0,0,0,1)*/
			vk::RenderingAttachmentInfo attachmentInfo{
				.imageView = swapChainImg.mImageView,
				.imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
				.loadOp = vk::AttachmentLoadOp::eClear,
				.storeOp = vk::AttachmentStoreOp::eStore,
				.clearValue = clearColor
			};

			//vk::RenderingAttachmentInfo depthAttachmentInfo{
			//	.imageView = depthImageView,
			//	.imageLayout = vk::ImageLayout::eDepthAttachmentOptimal,
			//	.loadOp = vk::AttachmentLoadOp::eClear,
			//	.storeOp = vk::AttachmentStoreOp::eDontCare,
			//	.clearValue = depthClearValue
			//};

			/*"color attachment" = the image where your pixel colors end up. Your swapchain image is the color attachment.*/

			/*renderArea — draw over the whole swapchain (starting at 0,0, full extent)
			layerCount — 1, you're not doing stereoscopic/VR stuff
			colorAttachmentCount + pColorAttachments — points to that attachment info above*/
			vk::RenderingInfo renderingInfo{
				.renderArea = {.offset = {0, 0}, .extent = *swapchainCtx.swapchainExtent},
				.layerCount = 1,
				.colorAttachmentCount = 1,
				.pColorAttachments = &attachmentInfo,
				//.pDepthAttachment = &depthAttachmentInfo
			};

			currentCommandBuff.beginRendering(renderingInfo);

			

			glm::mat4 proj = Geometry::projection(window, 100.f, 0.1f, 100.f);
			currentCommandBuff.bindPipeline(vk::PipelineBindPoint::eGraphics, *pipeline.getPipeline());
			currentCommandBuff.pushConstants<glm::mat4>(*pipeline.getLayout(), vk::ShaderStageFlagBits::eVertex, 0, proj);
			currentCommandBuff.bindVertexBuffers(0, meshManager.getUnderlyingBuffer(), {meshManager.getVertexMemoryInfo().getOffset()});
			currentCommandBuff.bindIndexBuffer(meshManager.getUnderlyingBuffer(), { meshManager.getIndicesMemoryInfo().getOffset() }, vk::IndexType::eUint32);

			//currentCommandBuff.bindVertexBuffers(0, *vertexBuffer, { 0 });
			//currentCommandBuff.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, *descriptorSets[frameIdx], nullptr);
			//currentCommandBuff.bindIndexBuffer(*indexBuffer, 0, vk::IndexType::eUint32);

			currentCommandBuff.setViewport(0, vk::Viewport(0.f, 0.f, static_cast<float>(swapchainCtx.swapchainExtent->width), static_cast<float>(swapchainCtx.swapchainExtent->height), 0.f, 1.f));
			currentCommandBuff.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), *swapchainCtx.swapchainExtent));


			//size_t totalGameObjsP{ gameObjects.gameObjInfoCollection.size() };
			//for (size_t i{}; i < totalGameObjsP; ++i) {
			//	currentCommandBuff.pushConstants<int>(pipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, static_cast<int>(i));
			//	currentCommandBuff.drawIndexed(gameObjects.verticesInformation[gameObjects.meshIdx[i]].totalIndices, 1, gameObjects.verticesInformation[gameObjects.meshIdx[i]].start - gameObjects.indices.begin(), 0, 0);
			//}

			//currentCommandBuff.draw(meshManager.getVertexCount(), 1, 0, 0);
			currentCommandBuff.drawIndexed(meshManager.getIndicesCount(), 1, 0, 0, 0);


			currentCommandBuff.endRendering();

			/*

				UNDEFINED → COLOR_ATTACHMENT_OPTIMAL

				"I don't care what state this image was in, just make it ready for drawing"
				Happens before beginRendering
				COLOR_ATTACHMENT_OPTIMAL (during rendering)

				GPU is actively writing pixels to it — your triangle gets drawn here
				COLOR_ATTACHMENT_OPTIMAL → PRESENT_SRC_KHR

				"Done drawing, rearrange it for showing on screen"
				Happens after endRendering
				PRESENT_SRC_KHR (presenting)

				Image gets sent to the display

			*/

			swapChainImg.transitionImageLayout(vk::ImageLayout::eColorAttachmentOptimal, vk::ImageLayout::ePresentSrcKHR, vk::AccessFlagBits2::eColorAttachmentWrite, {}, vk::PipelineStageFlagBits2::eColorAttachmentOutput, vk::PipelineStageFlagBits2::eBottomOfPipe, vk::ImageAspectFlagBits::eColor, currentCommandBuff);

			currentCommandBuff.end();


		}
		void Renderer::init(Core::SwapchainContext& swapchainCtx, Core::VulkanContext& vulkanCtx) {
		
			mCommandBuffer = Wrapper::CommandBuffer(*vulkanCtx.device, MAX_FRAMES_IN_FLIGHT, vulkanCtx.graphicsQueueIdx);

			mTransferFence = vk::raii::Fence(*vulkanCtx.device, vk::FenceCreateInfo{ });


			std::size_t size{ swapchainCtx.swapChainImages->getSize() };

			for (size_t i{}; i < size; ++i) {

				mRenderFinished.emplace_back(vk::raii::Semaphore(*vulkanCtx.device, vk::SemaphoreCreateInfo{}));
			
			}

			for (int i{}; i < MAX_FRAMES_IN_FLIGHT; ++i) {

				mFenceCollection.emplace_back(vk::raii::Fence(*vulkanCtx.device, vk::FenceCreateInfo{ .flags = vk::FenceCreateFlagBits::eSignaled }));
				mPresentFinished.emplace_back(vk::raii::Semaphore(*vulkanCtx.device, vk::SemaphoreCreateInfo{}));

			}

		}
	}
	
}


/*

Frame 0 (frame idx 0)
Wait for fence 0 (already default signaled)
Acquired swapchain image 0 (present semaphore 0 signal)
Reset fence 0
Submit for render image 0 (Signal render finished semaphore 0, wait for present semaphore 0)
Submit for present image 0 (Wait for render finished semaphore 0)

Frame 1 (frame idx 1)
Wait for fence 1 (already default signaled)
Acquired swapchain image 1 (present semaphore 1 signal)
Reset fence 1
Submit for render image 1 (Signal render finished semaphore 1, wait for present semaphore 1)
Submit for present image 1 (Wait for render finished semaphore 1)

Frame 2 (frame idx 0)
Wait for fence 0
Acquired swapchain image 2 (present semaphore 0 signal)
Reset fence 0
Submit for render image 2 (Signal render finished semaphore 2, wait for present semaphore 0)
Submit for present image 2 (Wait for render finished semaphore 0)

*/
