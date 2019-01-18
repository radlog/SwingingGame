#pragma once
class Allocator
{
public:
	Allocator();
	~Allocator();

	virtual void* allocate(size_t bytes) = 0;
	virtual void free(void* address);
};

