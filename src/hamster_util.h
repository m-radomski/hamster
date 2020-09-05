/* date = September 5th 2020 2:34 pm */

#ifndef HAMSTER_UTIL_H
#define HAMSTER_UTIL_H

#define ALLOC_CHUNK_SIZE 32
#define ARRAY_LEN(a) ((sizeof((a)))/(sizeof((a[0]))))
#define FLAG_SET(flag, value, type) ((flag) = ((type)((flag) | (value))))
#define FLAG_UNSET(flag, value, type) ((flag) = ((type)((flag) & ~(value))))

#define KB(x) (1024 * (x))
#define MB(x) (1024 * (KB(x)))
#define GB(x) (1024 * (MB(x)))

template <typename T, typename R>
struct Map
{
	T* key;
	R *value;
	unsigned int length;
	unsigned int capacity;
	
	Map() : 
	length(0), capacity(53)
	{ 
		key = (T *)malloc(sizeof(T) * capacity);
		value = (R *)malloc(sizeof(R) * capacity);
	}
	
	~Map()
	{
		free(key);
		free(value);
	}
	
	void clear()
	{
		memset(this->key, sizeof(T) * this->length);
		memset(this->value, sizeof(R) * this->length);
		this->length = 0;
	}
	
	bool contains(T key)
	{
		for(u32 i = 0; i < this->length; i++)
		{
			if(this->key[i] == key)
			{
				return true;
			}
		}
		
		return false;
	}
	
	void reserve(unsigned int size)
	{
		this->capacity += size;
		this->key = (T *)realloc(key, capacity * sizeof(T));
		assert(this->key);
		this->value = (R *)realloc(value, capacity * sizeof(R));
		assert(this->value);
	}
	
	
	R &operator[](T key)
	{
		u32 i = 0;
		while(i < this->length && !(this->key[i] == key))
		{
			i++;
		}
		
		this->length++;
		if(this->length == this->capacity)
		{
			this->reserve(ALLOC_CHUNK_SIZE);
		}
		
		this->key[i] = key;
		return this->value[i];
	}
};

struct Timer
{
    f64 frame_start;
    f64 frame_end;
    f64 since_last_second;
    u32 frames;
};

#endif //HAMSTER_UTIL_H
