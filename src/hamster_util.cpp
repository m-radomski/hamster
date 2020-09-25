static bool
is_digit(char a)
{
    return a >= '0' && a <= '9';
}

static bool
is_whitespace(char a)
{
    return a == ' ' || a == '\n' || a == '\t';
}

static char *
read_file_to_string(FILE *f)
{
    fseek(f, 0, SEEK_END);
    u32 file_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    char *result = (char *)malloc(file_size + 1);
    fread(result, file_size, 1, f);
    result[file_size] = '\0';
    return result;
}

static bool
strings_match(const char *str1, const char *str2)
{
    while(*str1 == *str2 && *str1 && *str2)
    {
        str1++; str2++;
    }
    return *str1 == *str2;
}

static bool
string_empty(const char *str)
{
    return str[0] == '\0';
}

static bool
string_starts_with(const char *str, const char *start)
{
    u32 str_len = strlen(str);
    u32 start_len = strlen(start);
    if(str_len < start_len)
    {
        return false;
    }
    
    return strncmp(str, start, start_len) == 0;
}

// NOTE(mateusz): Modifies the string in place, putting null-termination
// in places of the delimiter, returns the amount of elements that you can work on.
static u32
string_split(char *str, char delimiter)
{
    u32 parts = 1;
    
    while(*str)
    {
        if(*str == delimiter)
        {
            *str = '\0';
            parts++;
        }
        str++;
    }
    
    return parts;
}

// NOTE(mateusz): Helper for the string_split function. The user is supposed to
// know that it can safely ask for the next split element.
static char *
string_split_next(char *str)
{
    char *result = str + strlen(str) + 1;
    
    return result;
}

static u32
string_split_count_starting(char *lines, u32 lines_count, const char *start)
{
    u32 result = 0;
    
    char *line = lines;
    for(u32 i = 0; i < lines_count; i++)
    {
        if(string_starts_with(line, start))
        {
            result++;
        }
        line = string_split_next(line);
    }
    
    return result;
}

static void
string_find_and_replace(char *str, char find, char replace)
{
    char *newline = strchr(str, find);
    if(newline)
    {
        *newline = replace;
    }
}

// NOTE(mateusz): If the delimiter isn't found, copy the entire string
static void
string_copy_until(char *dest, char *src, char delim)
{
    while(*src)
    {
        if(*src != delim)
        {
            *(dest++) = *(src++);
        }
        else
        {
            *dest = '\0';
            break;
        }
    }
}

// NOTE(mateusz): These are not really that error checking, just made to be fast.
static f32
string_to_float(char *str)
{
    bool negative = false;
    if(str[0] == '-')
    {
        negative = true;
        str++;
    }
    
    f32 result = 0.0f;
    f32 base = 0.1f;
    bool after_decimal = false;
    while(*str)
    {
        if(*str != '.')
        {
            if(!after_decimal)
            {
                result = result * 10.0f + (f32)(*str - '0');
            }
            else
            {
                result += (f32)(*str - '0') * base;
                base /= 10.0f;
            }
        }
        else
        {
            after_decimal = true;
        }
        
        str++;
    }
    
    return negative ? -result : result;
}

static i32
string_to_int(char *str)
{
    bool negative = false;
    if(str[0] == '-')
    {
        negative = true;
        str++;
    }
    
    i32 result = 0;
    while(*str)
    {
        result = result * 10 + *str - '0';
        str++;
    }
    
    return negative ? -result : result;
}

static u32
string_get_char_count(char *str, char c)
{
    u32 result = 0;
    while(*str)
    {
        if(*str == c)
        {
            result++;
        }
        str++;
    }
    
    return result;
}

static void
sort(void *array, u32 count, u32 elem_size, bool (* swap_func)(void *, void *))
{
    // TODO(mateusz): Quick sort
    assert(elem_size < 512);
    u8 swap_buffer[512] = {};
    
    for(u32 i = 0; i < count - 1; i++)
    {
        for(u32 j = 0; j < count - 1; j++)
        {
            void *elem0 = (u8 *)array + j * elem_size;
            void *elem1 = (u8 *)array + (j + 1) * elem_size;
            if(swap_func(elem0, elem1))
            {
                memcpy(swap_buffer, elem0, elem_size);
                memcpy(elem0, elem1, elem_size);
                memcpy(elem1, swap_buffer, elem_size);
            }
        }
    }
}

static time_t
get_file_stamp(const char *filename)
{
    // NOTE(mateusz): Unix systems only!
#ifdef __linux__
    struct stat s = {};
    stat(filename, &s);
    return s.st_mtim.tv_sec;
#else
    (void)filename;
    printf("get_file_stamp not implemeted for this platform [%s : %d]\n", __FILE__, __LINE__);
    return 0;
#endif
}

static void 
editor_tick(ProgramState *state)
{
    if(!state->in_editor) { return; }
    
    EditorPickedEntity *picked = &state->edit_picked;
    if(!state->mbuttons[GLFW_MOUSE_BUTTON_LEFT].down && FLAG_IS_SET(picked->click_state, CLICKED_HOLDING))
    {
        printf("No longer holding the axis\n");
        FLAG_SET(picked->click_state, CLICKED_LET_GO);
        FLAG_UNSET(picked->click_state, CLICKED_HOLDING);
    }
    
    if(state->edit_picked.entity)
    {
        Entity *entity = state->edit_picked.entity;
        
        NOT_USED(entity);
    }
}

#if 0
template <typename T>
struct Array
{
	T* data;
	unsigned int length;
	unsigned int capacity;
	
	Array();
	~Array();
	
	void push(T val);
	void push_array(T *array, u32 size);
	T pop();
	void clear();
	
	bool contains(T val);
	
	void reserve(unsigned int size);
	T &operator[](unsigned int index);
};

template <typename T>
Array<T>::Array():
data(nullptr), length(0), capacity(0)
{ }

template <typename T>
Array<T>::~Array()
{
	free(data);
}

template <typename T>
void Array<T>::push(T val)
{
	if(length == capacity)
	{
		capacity += ALLOC_CHUNK_SIZE;
		data = (T *)realloc(data, capacity * sizeof(T));
		assert(data);
	}
	
	data[length++] = val;
}

template <typename T>
void Array<T>::push_array(T *array, u32 size)
{
	for(u32 i = 0; i < size; ++i)
	{
		this->push(array[i]);
	}
}

template <typename T>
T Array<T>::pop()
{
	assert(length != 0);
	return data[--length];
}

template <typename T>
void Array<T>::clear()
{
	memset(this->data, 0, sizeof(T) * this->length);
	this->length = 0;
}

template <typename T>
void Array<T>::reserve(unsigned int size)
{
	capacity += size;
	data = (T *)realloc(data, capacity * sizeof(T));
	assert(data);
}

template <typename T>
T &Array<T>::operator[](unsigned int index)
{
	assert(index <= length - 1);
	return data[index];
}

template <typename T>
bool Array<T>::contains(T val)
{
	bool result = false;
	
	for(unsigned int i = 0; i < length; i++)
	{
		if(data[i] == val)
		{
			result = true;
			break;
		}
	}
	
	return result;
}
#endif