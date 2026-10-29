#ifndef WRAPPER_STAGING_BUFFER_HPP
#define WRAPPER_STAGING_BUFFER_HPP


/**
 * @file stagingBuffer.hpp
 * @brief Declares Celer::Wrapper::StagingBuffer, a CPU-side staging buffer
 *        for uploading data to host-visible Vulkan memory.
 *
 * StagingBuffer accumulates data in a local std::vector<T> and copies it into
 * the mapped memory of a Wrapper::HostBuffer on demand.
 *
 * The class is move-only, since it owns a Vulkan buffer resource.
 *
 * @author Tristan
 * @date   2026-10-13
 * @version 1.0
 *
 * @see Celer::Wrapper::HostBuffer
 * @see Celer::Core::VulkanContext
 */


#include "hostVisibleBuffer.hpp"


namespace Celer {

	namespace Wrapper {

		/**
		 * @brief CPU-side staging buffer that accumulates data locally(through a vector) before
		 *        uploading it to a host-visible Vulkan buffer.
		 *
		 * The class is move-only. Copying is disabled because it owns a Vulkan
		 * buffer resource.
		 *
		 * @tparam T Element type stored in the buffer (e.g. a vertex or
		 *         uniform struct). Should not be dynamic memory (vector containing pointer to dynamic data)
		 */
		template<typename T>
		class StagingBuffer {

		private:
			uint32_t mBufferSize{};              ///< Size of the underlying buffer.
			std::vector<T> mLocalContainer;      ///< CPU-side storage for pending data.
			Wrapper::HostBuffer mBuffer;         ///< Host-visible buffer that receives the data.

		public:
			/// @brief Copy construction is disabled (the buffer owns a Vulkan resource).
			StagingBuffer(StagingBuffer const& src) = delete;

			/// @brief Copy assignment is disabled (the buffer owns a Vulkan resource).
			StagingBuffer& operator=(StagingBuffer const& src) = delete;

			/**
			 * @brief Move constructor.
			 * @param src Staging buffer to move from. 
			 */
			StagingBuffer(StagingBuffer&& src) noexcept;

			/**
			 * @brief Move assignment operator.
			 * @param src Staging buffer to move from. 
			 * @return Reference to this object.
			 */
			StagingBuffer& operator=(StagingBuffer&& src) noexcept;


			/**
			 * @brief Constructs a staging buffer and allocates the underlying
			 *        vector data
			 *
			 * @param size       Size of the buffer to allocate (in bytes. the underlying vector container gets reserved and resized according to the size/sizeof(T)).
			 * @param vulkanCtx  Vulkan context used to create the buffer
			 */
			StagingBuffer(uint32_t size, Core::VulkanContext& vulkanCtx);

			/**
			 * @brief Appends the elements of a container to the local container.
			 *
			 * Nothing is written to the GPU-visible buffer until
			 * copyToMappedBuffer() is called.
			 *
			 * @tparam Iterable Any type that provides a begin and end (iterators according to the stl)
			 * @param iterable Container whose elements are appended.
			 */
			template<typename Iterable>
			void addToLocalContainer(Iterable const& iterable);

			/**
			 * @brief Appends data pointed to by a raw pointer to the local container.
			 *
			 * @param ptrData Pointer to the data to append.
			 *
			 * @warning The amount of data read from @p ptrData is not
			 *          conveyed by this signature. The caller must ensure it
			 *          points to valid memory of the size this function
			 *          expects, and this size is entailed by what is set in the construction of the object, StagingBuffer(size...)
			 */
			void addToLocalContainer(void* ptrData);

			/**
			 * @brief Copies the local container's contents into the mapped
			 *        host-visible buffer.
			 *
			 * @param offset Offset into the mapped buffer at which writing
			 *               begins. Defaults to 0.
			 *
			 * @note The local container's size plus @p offset must not exceed
			 *       the underlying buffer's capacity.
			 * @note This does not clear the local container; call
			 *       clearLocalContainer() if you want to reuse it.
			 */
			void copyToMappedBuffer(uint32_t offset = 0);

			/**
			 * @brief Returns the underlying Vulkan buffer handle.
			 *
			 * @return The vk::Buffer owned by the internal HostBuffer. The
			 *         handle remains owned by this object and must not be
			 *         destroyed by the caller.
			 */
			vk::Buffer getUnderlyingBuffer();

			/**
			 * @brief Clears all pending data from the local container.
			 *
			 * Does not affect data already copied into the mapped buffer.
			 */
			void clearLocalContainer();


			uint32_t getSizeInBytes();

		};

		template<typename T>
		StagingBuffer<T>::StagingBuffer(StagingBuffer&& src) noexcept : mBufferSize{ src.mBufferSize },
			mLocalContainer{ std::move(src.mLocalContainer) },
			mBuffer{ std::move(src.mBuffer) } {

		}

		template<typename T>
		StagingBuffer<T>& StagingBuffer<T>::operator=(StagingBuffer&& src) noexcept {
			if (this != &src) {

				mBufferSize = src.mBufferSize;
				mBuffer = std::move(src.mBuffer);
				mLocalContainer = std::move(src.mLocalContainer);
			}

			return *this;
		}

		template<typename T>
		StagingBuffer<T>::StagingBuffer(uint32_t byteSize, Core::VulkanContext& vulkanCtx) : mBufferSize{ byteSize },
			mBuffer(byteSize, vk::BufferUsageFlagBits::eTransferSrc, vulkanCtx) {

			uint32_t sizeInContainer{ byteSize / sizeof(T) };
			mLocalContainer.reserve(sizeInContainer);
			mLocalContainer.resize(sizeInContainer);
		}

		template<typename T>
		template<typename Iterable>
		void StagingBuffer<T>::addToLocalContainer(Iterable const& iterable) {
			auto begin{ iterable.begin() };

			for (auto i{ begin }; i < iterable.end(); ++i) {
				mLocalContainer.push_back(*i);
			}

		}

		template<typename T>
		inline void StagingBuffer<T>::addToLocalContainer(void* ptrData) {
			memcpy(mLocalContainer.data(), ptrData, mBufferSize);
		
		}

		template<typename T>
		void StagingBuffer<T>::copyToMappedBuffer(uint32_t offset) {
			uint8_t* cvtBytePointer = static_cast<uint8_t*>(mBuffer.getMappedMemory());
			memcpy(cvtBytePointer + offset, mLocalContainer.data(), mLocalContainer.size() * sizeof(*mLocalContainer.begin()));
		}

		template<typename T>
		inline vk::Buffer StagingBuffer<T>::getUnderlyingBuffer() {
			return mBuffer.getBuffer();
		}

		template<typename T>
		void StagingBuffer<T>::clearLocalContainer() {
			mLocalContainer.clear();
		}

		template<typename T>
		inline uint32_t StagingBuffer<T>::getSizeInBytes()
		{
			return mBufferSize;
		}

	}



}

#endif
