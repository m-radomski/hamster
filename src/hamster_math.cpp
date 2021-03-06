static u32
find_first_set_bit(u32 a) // least significant bit / lsb
{
	u32 result = 0;
	
#if GCC_COMPILE
	result = __builtin_ctz(a);
#else
	for(u32 i = 0; i < sizeof(u32) * 8; ++i)
	{
		if((a & (1 << i)))
		{
			result = i;
			
			break;
		}
	}
#endif
	
	return result;
}

static u64
rdtsc()
{
	u64 result = 0;
	
#if GCC_COMPILE
	result = __rdtsc();
#endif
	
	return result;
}

static u32
hash(u8 *bytes, u32 size, u32 prime, u32 range)
{
	u32 hash = 0;
	
	for(u32 i = 0; i < size; i++)
	{
		hash += powl(prime, size - (i + 1)) * bytes[i];
		hash = hash % range;
	}
	
	return hash;
}

static u32
double_hash(u8 *bytes, u32 size, u32 attempt, u32 range)
{
	u32 hash1 = hash(bytes, size, HASHTABLE_PRIME1, range);
	u32 hash2 = hash(bytes, size, HASHTABLE_PRIME2, range);
	
	return hash1 + ((hash2 + 1) * attempt) % range;
}

//Carmack's fast inverse square root
inline static f32
fast_inverse_sqrtf(f32 a)
{
	f32 result = 0.0f;
	i32 i = 0;
	f32 x = 0;
	f32 y = 0;
	f32 three_halfs = 1.5f;
	
	x = a * 0.5f;
	y = a;
	i = *(i32 *)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(f32 *)&i;
	y = y * (three_halfs - (x * y * y));
	y = y * (three_halfs - (x * y * y));
	
	result = y;
	
	return result;
}

//Carmack's fast inverse square root back to square root
inline static f32
fast_sqrtf(f32 a)
{
	f32 result = 0.0f;
	
	result = a * fast_inverse_sqrtf(a);
	
	return result;
}

static u32
xorshift32(RandomSeries *series)
{
	u32 result = 0;
	
	u32 x = series->v;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	
	series->v = result = x;
	
	return result;
}

inline static f32
random_unilateral(RandomSeries *series)
{
	f32 result = 0.0f;
	
	result = (f32)xorshift32(series) / U32MAX;
	
	return result;
}

inline static f32 
random_bilateral(RandomSeries *series)
{
	f32 result = 0.0f;
	
	result = random_unilateral(series) * 2.0f - 1.0f;
	
	return result;
}

inline static f32
lerp(f32 a, f32 t, f32 b)
{
	f32 result = 0.0f;
	
	result = (1.0f - t) * a + t * b;
	
	return result;
}

inline static f32
square(f32 a)
{
    f32 result = 0.0f;
    
    result = a * a;
    
    return result;
}

inline static f32 
to_radians(f32 a)
{
	f32 result = 0.0f;
	
	result = (a / 180.0f) * PI;
	
	return result;
}

static f32
to_degrees(f32 a)
{
    f32 result = 0.0f;
    
    result = (a * 180.0f) / PI;
    
    return result;
}

inline static u32
clamp(u32 a, u32 min, u32 max)
{
	u32 result = a;
	
	result = MAX(min, result);
	result = MIN(max, result);
	
	return result;
}

inline static f32
clamp(f32 a, f32 min, f32 max)
{
	f32 result = a;
	
	result = MAX(min, result);
	result = MIN(max, result);
	
	return result;
}

inline static f32 
clamp01(f32 a)
{
	f32 result = a;
	
	result =	MAX(0.0f, result);
	result =	MIN(1.0f, result);
	
	return result;
}

inline static Vec2
add(Vec2 a, Vec2 b)
{
    Vec2 result;
    
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    
    return result;
}

inline static Vec2
adds(Vec2 a, f32 scalar)
{
    Vec2 result;
    
    result.x = a.x + scalar;
    result.y = a.y + scalar;
    
    return result;
}

inline static Vec2
sub(Vec2 a, Vec2 b)
{
    Vec2 result;
    
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    
    return result;
}

inline static Vec2
subs(Vec2 a, f32 scalar)
{
    Vec2 result = { 0 };
    
    result.x = a.x - scalar;
    result.y = a.y - scalar;
    
    return result;
}

inline static Vec2
div(Vec2 a, Vec2 b)
{
    Vec2 result = { 0 };
    
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    
    return result;
}

inline static Vec2
divs(Vec2 a, f32 scalar)
{
    Vec2 result = { 0 };
    
    result.x = a.x / scalar;
    result.y = a.y / scalar;
    
    return result;
}

inline static Vec2
hadamard(Vec2 a, Vec2 b)
{
    Vec2 result = { 0 };
    
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    
    return result;
}

inline static Vec2
scale(Vec2 a, f32 scalar)
{
    Vec2 result = { 0 };
    
    result.x = a.x * scalar;
    result.y = a.y * scalar;
    
    return result;
}

inline static f32
len(Vec2 a)
{
    f32 result = 0.0f;
    
    result = a.x * a.x + a.y * a.y;
    
    result = sqrtf(result);
    
    return result;
}

inline static f32
inverse_len(Vec2 a)
{
    f32 result = 0.0f;
    
    result = a.x * a.x + a.y * a.y;
    
    result = 1.0f / sqrtf(result);
    
    return result;
}

inline static Vec2
noz(Vec2 a)
{
    Vec2 result;
    f32 vec_len = 0.0f;
    
    vec_len = inverse_len(a);
    
    result = scale(a, vec_len);
    
    return result;
}

inline static Vec2 
negate(Vec2 a)
{
    Vec2 result;
    
    result.x = -a.x;
    result.y = -a.y;
    
    return result;
}

inline static Vec2
abs(Vec2 a)
{
    Vec2 result = {};
    
    result.x = fabsf(a.x);
    result.y = fabsf(a.y);
    
    return result;
}

inline static Vec2 
lerp(Vec2 a, f32 t, Vec2 b)
{
    Vec2 result = { 0 };
    
    result.x = (1.0f - t) * a.x + t * b.x;
    result.y = (1.0f - t) * a.y + t * b.y;
    
    return result;
}

inline static Vec2 
perp(Vec2 a)
{
    Vec2 result = { .x = -a.y, .y = a.x };
    
    return result;
}

inline static f32
inner(Vec2 a, Vec2 b)
{
    f32 result = 0.0f;
    
    result = a.x * b.x + a.y * b.y;
    
    return result;
}

inline static Vec3 
add(Vec3 a, Vec3 b)
{
    Vec3 result = { 0 };
    
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    
    return result;
}

inline static Vec3 
adds(Vec3 a, f32 scalar)
{
    Vec3 result = { 0 };
    
    result.x = a.x + scalar;
    result.y = a.y + scalar;
    result.z = a.z + scalar;
    
    return result;
}

inline static Vec3 
sub(Vec3 a, Vec3 b)
{
    Vec3 result = { 0 };
    
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    
    return result;
}

inline static Vec3 
subs(Vec3 a, f32 scalar)
{
    Vec3 result = { 0 };
    
    result.x = a.x - scalar;
    result.y = a.y - scalar;
    result.z = a.z - scalar;
    
    return result;
}

inline static Vec3 
div(Vec3 a, Vec3 b)
{
    Vec3 result = { 0 };
    
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
    
    return result;
}

inline static Vec3 
divs(Vec3 a, f32 scalar)
{
    Vec3 result = { 0 };
    
    result.x = a.x / scalar;
    result.y = a.y / scalar;
    result.z = a.z / scalar;
    
    return result;
}

inline static Vec3 
hadamard(Vec3 a, Vec3 b)
{
    Vec3 result = { 0 };
    
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    
    return result;
}

inline static Vec3 
scale(Vec3 a, f32 scalar)
{
    Vec3 result = { 0 };
    
    result.x = a.x * scalar;
    result.y = a.y * scalar;
    result.z = a.z * scalar;
    
    return result;
}

inline static Vec3
cross(Vec3 a, Vec3 b)
{
    Vec3 result = { 0 };
    
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    
    return result;
}

inline static f32 
len(Vec3 a)
{
    f32 result = 0.0f;
    
    result = a.x * a.x + a.y * a.y + a.z * a.z;
    
    result = sqrtf(result);
    
    return result;
}

inline static f32
inverse_len(Vec3 a)
{
    f32 result = 0.0f;
    
    result = a.x * a.x + a.y * a.y + a.z * a.z;
    
    result = 1.0f / sqrtf(result);
    
    return result;
}

inline static Vec3 
noz(Vec3 a)
{
    Vec3 result = { 0 };
    f32 vec_len = 0.0f;
    
    vec_len = inverse_len(a);
    
    result = scale(a, vec_len);
    
    return result;
}

inline static Vec3 
negate(Vec3 a)
{
    Vec3 result = { 0 };
    
    result.x = -a.x;
    result.y = -a.y;
    result.z = -a.z;
    
    return result;
}

inline static Vec3
abs(Vec3 a)
{
    Vec3 result = {};
    
    result.x = fabsf(a.x);
    result.y = fabsf(a.y);
    result.z = fabsf(a.z);
    
    return result;
}

inline static Vec3
lerp(Vec3 a, f32 t, Vec3 b)
{
    Vec3 result = { 0 };
    
    result.x = (1.0f - t) * a.x + t * b.x;
    result.y = (1.0f - t) * a.y + t * b.y;
    result.z = (1.0f - t) * a.z + t * b.z;
    
    return result;
}

inline static f32 
inner(Vec3 a, Vec3 b)
{
    f32 result = 0;
    
    result = a.x * b.x + a.y * b.y + a.z * b.z;
    
    return result;
}

inline static Vec4 
add(Vec4 a, Vec4 b)
{
    Vec4 result = { 0 };
    
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    result.w = a.w + b.w;
    
    return result;
}

inline static Vec4 
adds(Vec4 a, f32 scalar)
{
    Vec4 result = { 0 };
    
    result.x = a.x + scalar;
    result.y = a.y + scalar;
    result.z = a.z + scalar;
    result.w = a.w + scalar;
    
    return result;
}

inline static Vec4
sub(Vec4 a, Vec4 b)
{
    Vec4 result = { 0 };
    
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    result.z = a.w - b.w;
    
    return result;
}

inline static Vec4
subs(Vec4 a, f32 scalar)
{
    Vec4 result = { 0 };
    
    result.x = a.x - scalar;
    result.y = a.y - scalar;
    result.z = a.z - scalar;
    result.w = a.w - scalar;
    
    return result;
}

inline static Vec4
div(Vec4 a, Vec4 b)
{
    Vec4 result = { 0 };
    
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
    result.w = a.w / b.w;
    
    return result;
}

inline static Vec4
divs(Vec4 a, f32 scalar)
{
    Vec4 result = { 0 };
    
    result.x = a.x / scalar;
    result.y = a.y / scalar;
    result.z = a.z / scalar;
    result.w = a.w / scalar;
    
    return result;
}

inline static Vec4
hadamard(Vec4 a, Vec4 b)
{
    Vec4 result = { 0 };
    
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    result.w = a.w * b.w;
    
    return result;
}

inline static Vec4
scale(Vec4 a, f32 scalar)
{
    Vec4 result = { 0 };
    
    result.x = a.x * scalar;
    result.y = a.y * scalar;
    result.z = a.z * scalar;
    result.w = a.w * scalar;
    
    return result;
}

inline static f32 
len(Vec4 a)
{
    f32 result = 0.0f;
    
    result = a.x * a.x + a.y * a.y + a.z * a.z * a.w * a.w;
    
    result = sqrtf(result);
    
    return result;
}

inline static f32
inverse_len(Vec4 a)
{
    f32 result = 0.0f;
    
    result = a.x * a.x + a.y * a.y + a.z * a.z * a.w * a.w;
    
    result = 1.0f / sqrtf(result);
    
    return result;
}

inline static Vec4
noz(Vec4 a)
{
    Vec4 result = { 0 };
    f32 vec_len = 0.0f;
    
    vec_len = inverse_len(a);
    
    result = scale(a, vec_len);
    
    return result;
}

inline static Vec4
negate(Vec4 a)
{
    Vec4 result = { 0 };
    
    result.x = -a.x;
    result.y = -a.y;
    result.z = -a.z;
    result.w = -a.w;
    
    return result;
}

inline static Vec4
abs(Vec4 a)
{
    Vec4 result = {};
    
    result.x = fabsf(a.x);
    result.y = fabsf(a.y);
    result.z = fabsf(a.z);
    result.w = fabsf(a.w);
    
    return result;
}

inline static Vec4
cross(Vec4 a, Vec4 b)
{
    Vec4 result = { 0 };
    
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    
    return result;
}

inline static Vec4
lerp(Vec4 a, f32 t, Vec4 b)
{
    Vec4 result = { 0 };
    
    result.x = (1.0f - t) * a.x + t * b.x;
    result.y = (1.0f - t) * a.y + t * b.y;
    result.z = (1.0f - t) * a.z + t * b.z;
    result.w = (1.0f - t) * a.w + t * b.w;
    
    return result;
}

inline static f32 
inner(Vec4 a, Vec4 b)
{
    f32 result = 0;
    
    result = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    
    return result;
}


inline static f32
len(Quat a)
{
    f32 result = 0.0f;
    result = sqrtf(a.w * a.w + a.v.x * a.v.x + a.v.y * a.v.y + a.v.z * a.v.z);
    return result;
}

inline static Quat
noz(Quat a)
{
    f32 l = len(a);
    Quat result = Quat(a.w / l, Vec3(a.v.x / l, a.v.y / l, a.v.z / l));
    return result;
}

inline static Quat
mul(Quat a, Quat b)
{
    Quat result = {};
    
    result.w = a.w * b.w - a.v.x * b.v.x - a.v.y * b.v.y - a.v.z * b.v.z;
    result.v.x = a.w * b.w + a.v.x * b.v.x + a.v.y * b.v.y - a.v.z * b.v.z;
    result.v.y = a.w * b.w - a.v.x * b.v.x + a.v.y * b.v.y + a.v.z * b.v.z;
    result.v.z = a.w * b.w + a.v.x * b.v.x - a.v.y * b.v.y + a.v.z * b.v.z;
    
    return result;
}

inline static Quat
lerp(Quat a, f32 t, Quat b)
{
    Quat result = { 0 };
    
    result.w = (1.0f - t) * a.w + t * b.w;
    result.v.x = (1.0f - t) * a.v.x + t * b.v.x;
    result.v.y = (1.0f - t) * a.v.y + t * b.v.y;
    result.v.z = (1.0f - t) * a.v.z + t * b.v.z;
    
    return result;
}

inline static Mat2 
add(Mat2 a, Mat2 b)
{
    Mat2 result = { 0 };
    
    for(u32 index = 0;
        index < MATRIX2_ELEMENTS;
        ++index)
    {
        result.a1d[index] = a.a1d[index] + b.a1d[index];
    }
    
    return result;
}

inline static Quat
create_qrot(f32 theta, Vec3 dir)
{
    dir = noz(dir);
    
    f32 half_theta = 0.5f * theta;
    f32 sin_theta = sinf(half_theta);
    f32 cos_theta = cosf(half_theta);
    Quat result = Quat(cos_theta, Vec3(dir.x * sin_theta, dir.y * sin_theta, dir.z * sin_theta));
    
    return noz(result);
}

inline static Quat
qrot(Quat a, f32 angle, Vec3 dir)
{
    Quat b = create_qrot(angle, dir);
    Quat result = mul(b, a);
    
    return result;
}

inline static Mat2 
adds(Mat2 a, f32 scalar)
{
    Mat2 result = { 0 };
    
    for(u32 index = 0;
        index < MATRIX2_ELEMENTS;
        ++index)
    {
        result.a1d[index] = a.a1d[index] + scalar;
    }
    
    return result;
}

inline static Mat2 
sub(Mat2 a, Mat2 b)
{
    Mat2 result = { 0 };
    
    for(u32 index = 0;
        index < MATRIX2_ELEMENTS;
        ++index)
    {
        result.a1d[index] = a.a1d[index] - b.a1d[index];
    }
    
    return result;
}

inline static Mat2 
subs(Mat2 a, f32 scalar)
{
    Mat2 result = { 0 };
    
    for(u32 index = 0;
        index < MATRIX2_ELEMENTS;
        ++index)
    {
        result.a1d[index] = a.a1d[index] - scalar;
    }
    
    return result;
}

inline static Mat2 
muls(Mat2 a, f32 scalar)
{
    Mat2 result = { 0 };
    
    for(u32 index = 0;
        index < MATRIX2_ELEMENTS;
        ++index)
    {
        result.a1d[index] = a.a1d[index] * scalar;
    }
    
    return result;
}

inline static Mat2 
mul(Mat2 a, Mat2 b)
{
    Mat2 result = { 0 };
    
    result.a[0][0] = a.a[0][0] * b.a[0][0] + a.a[1][0] * b.a[0][1];
    result.a[0][1] = a.a[0][1] * b.a[0][0] + a.a[1][1] * b.a[0][1];
    result.a[1][0] = a.a[0][0] * b.a[0][1] + a.a[1][0] * b.a[1][1];
    result.a[1][1] = a.a[0][1] * b.a[1][0] + a.a[1][1] * b.a[1][1];
    
    return result;
}

inline static Mat4
add(Mat4 a, Mat4 b)
{
    Mat4 result = { 0 };
    
    for(u32 index = 0;
        index < MATRIX4_ELEMENTS;
        ++index)
    {
        result.a1d[index] = a.a1d[index] + b.a1d[index];
    }
    
    return result;
}

inline static Mat4
adds(Mat4 a, f32 scalar)
{
    Mat4 result = { 0 };
    
    for(u32 index = 0;
        index < MATRIX4_ELEMENTS;
        ++index)
    {
        result.a1d[index] = a.a1d[index] + scalar;
    }
    
    return result;
}

inline static Mat4
sub(Mat4 a, Mat4 b)
{
    Mat4 result = { 0 };
    
    for(u32 index = 0;
        index < MATRIX4_ELEMENTS;
        ++index)
    {
        result.a1d[index] = a.a1d[index] + b.a1d[index];
    }
    
    return result;
}

inline static Mat4
subs(Mat4 a, f32 scalar)
{
    Mat4 result = { 0 };
    
    for(u32 index = 0;
        index < MATRIX4_ELEMENTS;
        ++index)
    {
        result.a1d[index] = a.a1d[index] + scalar;
    }
    
    return result;
}

inline static Mat4
scale(Mat4 a, f32 scalar)
{
    Mat4 result = { 0 };
    
    for(u32 index = 0;
        index < MATRIX4_ELEMENTS;
        ++index)
    {
        result.a1d[index] = a.a1d[index] * scalar;
    }
    
    
    return result;
}

inline static Mat4
muls(Mat4 a, f32 scalar)
{
    Mat4 result = { 0 };
    
    for(u32 index = 0;
        index < MATRIX4_ELEMENTS;
        ++index)
    {
        result.a1d[index] = a.a1d[index] * scalar;
    }
    
    return result;
}

inline static Mat4
mul(Mat4 a, Mat4 b)
{
    Mat4 result = { 0 };
    
    result.a[0][0] = a.a[0][0] * b.a[0][0] + a.a[1][0] * b.a[0][1] + a.a[2][0] * b.a[0][2] + a.a[3][0] * b.a[0][3];
    result.a[1][0] = a.a[0][0] * b.a[1][0] + a.a[1][0] * b.a[1][1] + a.a[2][0] * b.a[1][2] + a.a[3][0] * b.a[1][3];
    result.a[2][0] = a.a[0][0] * b.a[2][0] + a.a[1][0] * b.a[2][1] + a.a[2][0] * b.a[2][2] + a.a[3][0] * b.a[2][3];
    result.a[3][0] = a.a[0][0] * b.a[3][0] + a.a[1][0] * b.a[3][1] + a.a[2][0] * b.a[3][2] + a.a[3][0] * b.a[3][3];
    
    result.a[0][1] = a.a[0][1] * b.a[0][0] + a.a[1][1] * b.a[0][1] + a.a[2][1] * b.a[0][2] + a.a[3][1] * b.a[0][3];
    result.a[1][1] = a.a[0][1] * b.a[1][0] + a.a[1][1] * b.a[1][1] + a.a[2][1] * b.a[1][2] + a.a[3][1] * b.a[1][3];
    result.a[2][1] = a.a[0][1] * b.a[2][0] + a.a[1][1] * b.a[2][1] + a.a[2][1] * b.a[2][2] + a.a[3][1] * b.a[2][3];
    result.a[3][1] = a.a[0][1] * b.a[3][0] + a.a[1][1] * b.a[3][1] + a.a[2][1] * b.a[3][2] + a.a[3][1] * b.a[3][3];
    
    result.a[0][2] = a.a[0][2] * b.a[0][0] + a.a[1][2] * b.a[0][1] + a.a[2][2] * b.a[0][2] + a.a[3][2] * b.a[0][3];
    result.a[1][2] = a.a[0][2] * b.a[1][0] + a.a[1][2] * b.a[1][1] + a.a[2][2] * b.a[1][2] + a.a[3][2] * b.a[1][3];
    result.a[2][2] = a.a[0][2] * b.a[2][0] + a.a[1][2] * b.a[2][1] + a.a[2][2] * b.a[2][2] + a.a[3][2] * b.a[2][3];
    result.a[3][2] = a.a[0][2] * b.a[3][0] + a.a[1][2] * b.a[3][1] + a.a[2][2] * b.a[3][2] + a.a[3][2] * b.a[3][3];
    
    result.a[0][3] = a.a[0][3] * b.a[0][0] + a.a[1][3] * b.a[0][1] + a.a[2][3] * b.a[0][2] + a.a[3][3] * b.a[0][3];
    result.a[1][3] = a.a[0][3] * b.a[1][0] + a.a[1][3] * b.a[1][1] + a.a[2][3] * b.a[1][2] + a.a[3][3] * b.a[1][3];
    result.a[2][3] = a.a[0][3] * b.a[2][0] + a.a[1][3] * b.a[2][1] + a.a[2][3] * b.a[2][2] + a.a[3][3] * b.a[2][3];
    result.a[3][3] = a.a[0][3] * b.a[3][0] + a.a[1][3] * b.a[3][1] + a.a[2][3] * b.a[3][2] + a.a[3][3] * b.a[3][3];
    
    return result;
}

inline static Vec3
mul(Mat4 a, Vec3 v)
{
    Vec3 result = {};
    
    result.x = v.x * a.a[0][0] + v.y * a.a[1][0] + v.z * a.a[2][0] + a.a[3][0];
    result.y = v.x * a.a[0][1] + v.y * a.a[1][1] + v.z * a.a[2][1] + a.a[3][1];
    result.z = v.x * a.a[0][2] + v.y * a.a[1][2] + v.z * a.a[2][2] + a.a[3][2];
    
    return result;
}

inline static Vec4
mul(Mat4 a, Vec4 v)
{
    Vec4 result = {};
    
    result.x = v.x * a.a[0][0] + v.y * a.a[1][0] + v.z * a.a[2][0] + v.w * a.a[3][0];
    result.y = v.x * a.a[0][1] + v.y * a.a[1][1] + v.z * a.a[2][1] + v.w * a.a[3][1];
    result.z = v.x * a.a[0][2] + v.y * a.a[1][2] + v.z * a.a[2][2] + v.w * a.a[3][2];
    result.w = v.x * a.a[0][3] + v.y * a.a[1][3] + v.z * a.a[2][3] + v.w * a.a[3][3];
    
    return result;
}

static Mat4
inverse(Mat4 v)
{
    f32 a = v.a[0][0], e = v.a[1][0], i = v.a[2][0], m = v.a[3][0],
    b = v.a[0][1], f = v.a[1][1], j = v.a[2][1], n = v.a[3][1],
    c = v.a[0][2], g = v.a[1][2], k = v.a[2][2], o = v.a[3][2],
    d = v.a[0][3], h = v.a[1][3], l = v.a[2][3], p = v.a[3][3];
    
    f32 dfill[6] = {};
    dfill[0] = k * p - o * l; dfill[1] = j * p - l * n; dfill[2] = j * o - n * k;
    dfill[3] = i * p - m * l; dfill[4] = i * o - m * k; dfill[5] = i * n - m * j;
    
    Mat4 result = {};
    result.a[0][0] =   f * dfill[0] - g * dfill[1] + h * dfill[2];
    result.a[1][0] = -(e * dfill[0] - g * dfill[3] + h * dfill[4]);
    result.a[2][0] =   e * dfill[1] - f * dfill[3] + h * dfill[5];
    result.a[3][0] = -(e * dfill[2] - f * dfill[4] + g * dfill[5]);
    
    result.a[0][1] = -(b * dfill[0] - c * dfill[1] + d * dfill[2]);
    result.a[1][1] =   a * dfill[0] - c * dfill[3] + d * dfill[4];
    result.a[2][1] = -(a * dfill[1] - b * dfill[3] + d * dfill[5]);
    result.a[3][1] =   a * dfill[2] - b * dfill[4] + c * dfill[5];
    
    dfill[0] = g * p - o * h; dfill[1] = f * p - n * h; dfill[2] = f * o - n * g;
    dfill[3] = e * p - m * h; dfill[4] = e * o - m * g; dfill[5] = e * n - m * f;
    
    result.a[0][2] =   b * dfill[0] - c * dfill[1] + d * dfill[2];
    result.a[1][2] = -(a * dfill[0] - c * dfill[3] + d * dfill[4]);
    result.a[2][2] =   a * dfill[1] - b * dfill[3] + d * dfill[5];
    result.a[3][2] = -(a * dfill[2] - b * dfill[4] + c * dfill[5]);
    
    dfill[0] = g * l - k * h; dfill[1] = f * l - j * h; dfill[2] = f * k - j * g;
    dfill[3] = e * l - i * h; dfill[4] = e * k - i * g; dfill[5] = e * j - i * f;
    
    result.a[0][3] = -(b * dfill[0] - c * dfill[1] + d * dfill[2]);
    result.a[1][3] =   a * dfill[0] - c * dfill[3] + d * dfill[4];
    result.a[2][3] = -(a * dfill[1] - b * dfill[3] + d * dfill[5]);
    result.a[3][3] =   a * dfill[2] - b * dfill[4] + c * dfill[5];
    
    f32 det = 1.0f / (a * result.a[0][0] + b * result.a[1][0] + 
                      c * result.a[2][0] + d * result.a[3][0]);
    
    result = scale(result, det);
    
    return result;
}

static Mat4
create_perspective(f32 aspect_ratio, f32 fov, f32 near_z, f32 far_z)
{
    Mat4 result = { 0 };
    f32 tan_half_fov = 0.0f;
    
    tan_half_fov = tanf(to_radians(fov) * 0.5f);
    
    result.a[0][0] = 1.0f / (aspect_ratio * tan_half_fov);
    result.a[1][1] = 1.0f / tan_half_fov;
    result.a[2][2] = (near_z + far_z) / (near_z - far_z);
    result.a[2][3] = -1.0f;
    result.a[3][2] = (2.0f * near_z * far_z) / (near_z - far_z);
    
    return result;
}

static Mat4
create_orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far)
{
    Mat4 result = {};
    
    result.a[0][0] = 2.0f / (right - left);
    result.a[1][1] = 2.0f / (top - bottom);
    result.a[2][2] = -2.0f / (far - near);
    
    result.a[3][0] = -(right + left) / (right - left);
    result.a[3][1] = -(top + bottom) / (top - bottom);
    result.a[3][2] = -(far + near) / (far - near);
    result.a[3][3] = 1.0f;
    
    return result;
}

static Mat4
create_orthographic(f32 aspect_ratio, f32 near_z, f32 far_z)
{
    Mat4 result = {};
    
    result.a[0][0] = 1.0f / aspect_ratio;
    result.a[1][1] = 1.0f;
    result.a[2][2] = -2.0f / (far_z - near_z);
    result.a[3][2] = -(far_z + near_z) / far_z - near_z; 
    result.a[3][3] = 1.0f;
    
    return result;
}

static Mat4
look_at(Vec3 cam_target, Vec3 cam_pos, Vec3 cam_up)
{
    Mat4 result = { 0 };
    Vec3 look_at = { 0 };
    Vec3 right = { 0 };
    Vec3 up = { 0 };
    
    look_at = noz(sub(cam_target, cam_pos));
    right = noz(cross(look_at, cam_up));
    up = noz(cross(right, look_at));
    
    result.a[0][0] = right.x;
    result.a[1][0] = right.y;
    result.a[2][0] = right.z;
    result.a[3][0] = -inner(right, cam_pos);
    result.a[0][1] = up.x;
    result.a[1][1] = up.y;
    result.a[2][1] = up.z;
    result.a[3][1] = -inner(up, cam_pos);
    result.a[0][2] = -look_at.x;
    result.a[1][2] = -look_at.y;
    result.a[2][2] = -look_at.z;
    result.a[3][2] = inner(look_at, cam_pos);
    result.a[0][3] = result.a[1][3] = result.a[2][3] = 0;
    result.a[3][3] = 1.0f;
    
    return result;
}

inline static Mat4 
create_translate(Mat4 a, Vec3 trans)
{
    Mat4 result = { 0 };
    Vec4 colum0 = { 0 };
    Vec4 colum1 = { 0 };
    Vec4 colum2 = { 0 };
    
    result = a;
    
    colum0 = scale(a.columns[0], trans.x);
    colum1 = scale(a.columns[1], trans.y);
    colum2 = scale(a.columns[2], trans.z);
    
    result.columns[3] = add(colum0, result.columns[3]);
    result.columns[3] = add(colum1, result.columns[3]);
    result.columns[3] = add(colum2, result.columns[3]);
    
    return result;
}

inline static Mat4
translate(Mat4 a, Vec3 trans)
{
    Mat4 result = { 0 };
    
    result = create_translate(Mat4(1.0f), trans);
    result = mul(result, a);
    
    return result;
}

inline static Mat4 
scale(Mat4 a, Vec3 size)
{
    Mat4 result = { 0 };
    
    result.columns[0] = scale(a.columns[0], size.x);
    result.columns[1] = scale(a.columns[1], size.y);
    result.columns[2] = scale(a.columns[2], size.z);
    result.columns[3] = a.columns[3];
    
    return result;
}

static Mat4 
create_scale(Vec3 size)
{
    Mat4 result = { 0 };
    
    result.a[0][0] = size.x;
    result.a[1][1] = size.y;
    result.a[2][2] = size.z;
    result.a[3][3] = 1.0f;
    
    return result;
}

static Mat4
rot_around_vec(Mat4 a, f32 angle, Vec3 vec)
{
    Mat4 rotation_matrix = { 0 };
    
    vec = noz(vec);
    
    f32 cos_theta = cosf(angle);
    f32 sin_theta = sinf(angle);
    f32 one_minus_cos_theta = 1.0f - cos_theta;
    
    rotation_matrix.a[0][0] = cos_theta + one_minus_cos_theta * vec.x * vec.x;
    rotation_matrix.a[0][1] = one_minus_cos_theta * vec.x * vec.y + sin_theta * vec.z;
    rotation_matrix.a[0][2] = one_minus_cos_theta * vec.x * vec.z - sin_theta * vec.y;
    
    rotation_matrix.a[1][0] = one_minus_cos_theta * vec.x * vec.y - sin_theta * vec.z;
    rotation_matrix.a[1][1] = cos_theta + one_minus_cos_theta * vec.y * vec.y;
    rotation_matrix.a[1][2] = one_minus_cos_theta * vec.y * vec.z + sin_theta * vec.x;
    
    rotation_matrix.a[2][0] = one_minus_cos_theta * vec.z * vec.x + sin_theta * vec.y;
    rotation_matrix.a[2][1] = one_minus_cos_theta * vec.z * vec.y - sin_theta * vec.x;
    rotation_matrix.a[2][2] = cos_theta + one_minus_cos_theta * vec.z * vec.z;
    
    rotation_matrix.a[3][3] = 1.0f;
    
    return mul(rotation_matrix, a);
}

static Mat4 
rotate_from_quat(Quat a)
{
    Mat4 result = {};
    
    f32 qw = a.w;
    f32 qi = a.v.x;
    f32 qj = a.v.y;
    f32 qk = a.v.z;
    f32 twos = 2.0f / (qw*qw + qi*qi + qj*qj + qk*qk);
    
    result.a[0][0] = 1.0f - twos * (qj*qj + qk*qk);
    result.a[1][0] = twos * (qi*qj - qk*qw);
    result.a[2][0] = twos * (qi*qk + qj*qw);
    
    result.a[0][1] = twos * (qi*qj + qk*qw);
    result.a[1][1] = 1.0f - twos * (qi*qi + qk*qk);
    result.a[2][1] = twos * (qj*qk - qi*qw);
    
    result.a[0][2] = twos * (qi*qk - qj*qw);
    result.a[1][2] = twos * (qj*qk + qi*qw);
    result.a[2][2] = 1.0f - twos * (qi*qi + qj*qj);
    
    result.a[3][3] = 1.0;
    
    return result;
}

static Mat4 
rotate_quat(Mat4 a, Quat q)
{
    Mat4 new_matrix = rotate_from_quat(q);
    
    return mul(new_matrix, a);
}