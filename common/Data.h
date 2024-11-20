#pragma once
#include <iostream>

class Data
{
private:
	void* data = nullptr;
	size_t size = 0;

	uint32_t write_offset = 0;
	uint32_t read_offset = 0;

public:
	Data(size_t size)
		:size(size)
	{
		data = malloc(size);
		if (!data)
		{
			std::cerr << "Failed to allocate memory\n";
			exit(-1);
		}
	}

	~Data()
	{
		free(data);
	}

	bool put(void* source, size_t size)
	{
		uint32_t next_offset = write_offset + size;
		if (next_offset > this->size)
			return false;

		std::memcpy((char*)data + write_offset, source, size);
		write_offset = next_offset;
		return true;
	}

	bool put(Data* source)
	{
		return put(source->getData(), source->getSize());
	}

	bool get(void* destination, size_t size)
	{
		uint32_t next_offset = read_offset + size;
		if (next_offset > this->size)
			return false;

		std::memcpy(destination, (char*)data + read_offset, size);
		read_offset = next_offset;
		return true;
	}

	Data* get(size_t size)
	{
		Data* data = new Data(size);
		if (!get(data->getData(), size))
		{
			delete data;
			data = nullptr;
		}

		return data;
	}

	size_t getSize() { return size; }
	void* getData() { return data; }


};
