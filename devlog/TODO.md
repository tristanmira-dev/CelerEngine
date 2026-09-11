~~1. Actually finish descriptor sets for samplers~~
2. Depth buffers 
3. Fix allocator or make it into an abstraction (class)
4. migrate vertex upload to upload manager
5. look more into the inefficiencies of having separate descriptor sets for resources
~~6. yep, need more descriptors lol, for the uniform buffers and structured buffers at least, added a method and modified the pool creation, just need to integrate another set of buffers for the custom allocator~~
~~7. ok, next session, i need to finish the updateDescriptors, at least for the projection matrix which should be simple if things go well. i might have to do some finagling or design a way for the gameObject side to get texture and mesh data?~~
8.  maybe add a indices count and an offset that i can give the index to the game object manager? will see
9. yep need to refactor these wrappers that have some move logic, had insane bugs with the vector since the nature of it 
   (everytime it grows past its capacity it will reallocate a new one twice its size, copy over the contents from the old one and deallocate 
   memory and after which calls my destructor that did not have guards to check if its unmapping a buffer that still exists and yeah, good ol crashing. Also had issues with my upload manager
   that was holding onto stale pointers, because of said mechanism of the vector. I just hold onto the raw vulkan handle instead)