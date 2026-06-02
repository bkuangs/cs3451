#define Terrain 1		/* Uncomment this macro for Step II */

struct Light 
{
    vec3 position;          /* light position */
    vec3 Ia;                /* ambient intensity */
    vec3 Id;                /* diffuse intensity */
    vec3 Is;                /* specular intensity */     
};

uniform vec3 ka;            /* object material ambient */
uniform vec3 kd;            /* object material diffuse */
uniform vec3 ks;            /* object material specular */
uniform float shininess;    /* object material shininess */

/////////////////////////////////////////////////////
//// Step 1 - Part 1: Hash function
//// In this function, you will create a function that takes in an xy coordinate and returns a 'random' 2d vector.
//// You are asked to implement your own version by combining different GLSL built-in functions to produce the result.
//// You are allowed to leverage the reference Hash() implementation online (but you need to put the source link in comments). 
//// We also provide a default implementation of Hash() to obtain the image results shown in the assignments. 
//// You implementation does not need to match the reference results.
/////////////////////////////////////////////////////

vec2 hash2(vec2 v)
{
	vec2 rand = vec2(0,0);
	
	/* Your implementation starts */

	// Provided defulat implementation
	// rand  = 52.5 * fract(v.yx * 0.31 + vec2(0.31, 0.113));
    // rand = -1.0 + 3.1 * fract(rand.x * rand.y * rand.yx);

	vec3 v3 = fract(vec3(v.xyx) * vec3(443.897, 441.423, 437.195));
    v3 += dot(v3, v3.yzx + 19.19);
    rand = fract((v3.xx + v3.yz) * v3.zy);
	
	/* Your implementation ends */

	return rand;
}

/////////////////////////////////////////////////////
//// Step 1 - Part 2: Perlin Noise
//// In this function, you will implement the Perlin noise with a single octave.
//// The input is a 2D position p. We calculate the grid cell index i and fraction f. 
//// You will use i and f to compute the Perlin noise at point p and return it as noise.
/////////////////////////////////////////////////////

float perlin_noise(vec2 p) 
{
    float noise = 0.0;
	vec2 i = floor(p);
    vec2 f = fract(p);
	
	/* Your implementation starts */

	// gradient vectors
	vec2 g00 = normalize(hash2(i) * 2.0 - 1.0);
	vec2 g10 = normalize(hash2(i + vec2(1., 0.)) * 2.0 - 1.0);
	vec2 g01 = normalize(hash2(i + vec2(0., 1.)) * 2.0 - 1.0);
	vec2 g11 = normalize(hash2(i + vec2(1., 1.)) * 2.0 - 1.0);

	// direction vectors
	vec2 f00 = f - vec2(0.0, 0.0);
	vec2 f10 = f - vec2(1.0, 0.0);
	vec2 f01 = f - vec2(0.0, 1.0);
	vec2 f11 = f - vec2(1.0, 1.0);

	// dot products
	float v00 = dot(f00, g00);
	float v10 = dot(f10, g10);
	float v01 = dot(f01, g01);
	float v11 = dot(f11, g11);

	// horizontal interpolations
	float sx = smoothstep(0.0, 1.0, f.x);
	float sy = smoothstep(0.0, 1.0, f.y);

	float u0 = mix(v00, v10, sx);
	float u1 = mix(v01, v11, sx);

	// vertical interpolation
	noise = mix(u0, u1, sy);

	/* Your implementation ends */
	
	return noise;
}

/////////////////////////////////////////////////////
//// Step 1 - Part 3: Octave synthesis
//// In this function, you will synthesize the noise octave by invoking the perlin_noise function, which should be implemented in the previous step. 
//// Given a point p and an octave number num, the task is to compute the Perlin noise octave by accumulating the contributions from each frequency level. 
//// At each level, the amplitude should be halved, while the frequency should be doubled. 
//// The octave number num must be greater than 0.
/////////////////////////////////////////////////////

float noise_octave(vec2 p, int num)
{
	float sum = 0;
	
	/* Your implementation starts */

	float amplitude = 1.0;
	float frequency = 1.0;

	for (int i = 0; i < num; i++) {
		sum += amplitude * perlin_noise(frequency * p);
		amplitude *= 0.5;
		frequency *= 2.0;
	}
	
	/* Your implementation ends */
	
	return sum;
}

/////////////////////////////////////////////////////
//// Step 2 - Part 1: Calculate vertex height
//// Create a function that takes in a 2D point and returns its height using the noise_octave() funciton you have implemented.
//// There is no standard answer for this part. Think about what functions will create what shapes.
//// If you want steep mountains with flat tops, use a function like sqrt(noise_octave(v,num)). 
//// If you want jagged mountains, use a function like e^(noise_octave(v,num)).
//// You can also add functions on top of each other and change the frequency of the noise by multiplying v by some value other than 1.
//// In the starter code, we provide our default implementation for your reference.
/////////////////////////////////////////////////////

float volcano_height(vec2 v, vec2 c, float radius, float peak, float crater)
{
    float d = length(v - c);
    float fade = 1.0 - smoothstep(radius * 0.78, radius, d);

    float cone = peak * pow(max(0.0, 1.0 - d / radius), 1.35);
    float rim = 0.28 * peak * exp(-pow((d - crater) / (crater * 0.35), 2.0));
    float bowl = 0.58 * peak * (1.0 - smoothstep(crater * 0.30, crater, d));

    float ridges = 0.10 * peak * noise_octave((v - c) * 4.0, 5);
    float fine_rock = 0.035 * peak * noise_octave(v * 15.0 + c, 3);

    return fade * (cone + rim - bowl + ridges + fine_rock);
}

float crater_lava_mask(vec2 v, vec2 c, float crater)
{
    float d = length(v - c);

    float pool = 1.0 - smoothstep(crater * 0.35, crater * 0.82, d);
    float rim = clamp(1.0 - abs(d - crater) / (crater * 0.23), 0.0, 1.0);

    float broken = smoothstep(
        0.18,
        0.72,
        0.5 + 0.5 * noise_octave(v * 18.0 + c * 5.0, 4)
    );

    return clamp(pool + 0.35 * rim * broken, 0.0, 1.0);
}

float volcano_lava(vec2 v)
{
    float m = 0.0;
    m = max(m, crater_lava_mask(v, vec2(2.45, 2.55), 0.34));
    m = max(m, crater_lava_mask(v, vec2(1.15, 3.65), 0.22));
    m = max(m, crater_lava_mask(v, vec2(3.75, 1.35), 0.24));
    return m;
}

float height(vec2 v)
{
	float h = 0;

	/* Your implementation starts */

    h = -0.18;

    h += 0.08 * noise_octave(v * 1.2, 5);
    h += 0.025 * noise_octave(v * 7.0, 4);

    h += volcano_height(v, vec2(2.45, 2.55), 2.05, 1.55, 0.34);
    h += volcano_height(v, vec2(1.15, 3.65), 1.20, 0.85, 0.22);
    h += volcano_height(v, vec2(3.75, 1.35), 1.35, 0.95, 0.24);

	/* Your implementation ends */

    return h;
}

/////////////////////////////////////////////////////
//// Step 2 - Part 2: Compute normal for a given 2D point using its height specified by noise function
//// In this function, you are asked to create a function that takes in a 2D point and returns its normal
//// You need to compute the normal vector at p by find the points d to the left/right and d forward/backward, and then use a cross product to calculate the normal vector. 
//// Be sure to normalize the result after you calculate the cross product.
//// This function will be called in shading_terrain to calculate the normal vector to be used in the shading model.
/////////////////////////////////////////////////////

vec3 compute_normal(vec2 v, float d)
{	
	vec3 normal_vector = vec3(0,0,0);
	
	/* Your implementation starts */

	vec2 right = vec2(v.x + d, v.y);
	vec2 left = vec2(v.x - d, v.y);
	vec2 up = vec2(v.x, v.y + d);
	vec2 down = vec2(v.x, v.y - d);

	vec3 xtan = vec3(2.0 * d, 0., height(right) - height(left));
	vec3 ytan = vec3(0., 2.0 * d, height(up) - height(down));

	normal_vector = normalize(cross(xtan, ytan));
	
	/* Your implementation ends */
	
	return normal_vector;
}

/////////////////////////////////////////////////////
//// Step 2 - Part 3: Phong shading
//// In this function, you will implement the Phong shading model to be used to shade your mountain.
//// It is the standard version we have practiced in our previous assignments, and you are allowed to 
//// reuse the code you have implemented previously.
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////
//// Input variables for shading_phong
/////////////////////////////////////////////////////
//// light: the light struct
//// e: eye position
//// p: position of the point
//// s: light source position (you may also use light.position)
//// n: normal at the point
/////////////////////////////////////////////////////

vec4 shading_phong(Light light, vec3 e, vec3 p, vec3 s, vec3 n) 
{
	vec4 color = vec4(0.0,0.0,0.0,1.0);
	
    /* your implementation starts */

    vec3 l = normalize(s - p);
    vec3 v = normalize(e - p);
    vec3 r = reflect(-l, n);

    float diffuse = max(0., dot(n, l));
    float specular = pow(max(0., dot(r, v)), shininess);

    vec3 phong = (ka * light.Ia) + (kd * light.Id * diffuse) + (ks * light.Is * specular);

    color = vec4(phong, 1.f);
    
	/* your implementation ends */
	
	return color;
}

//// shade the noise function 
vec3 shading_noise(vec3 p) 
{
	float h = 0.5 + 0.5 * (noise_octave(p.xy, 4));
	return vec3(h, h, h);
}

/////////////////////////////////////////////////////
//// Step 2 - Part 4: Shade the terrain
//// In this function, you will calculate the emissive color of each input position
//// We provide a default implementation that is commented out by default
//// You are asked to implement your own version to calculate natural colors for your customized scene
/////////////////////////////////////////////////////

vec3 shading_terrain(vec3 pos)
{
/*
    const Light light = Light(vec3(3, 1, 3), vec3(1, 1, 1), vec3(2, 2, 2), vec3(1, 1, 1));

    //// calculate Phong shading color with normal
    
    vec3 n = compute_normal(pos.xy, 0.01);
    vec3 e = position.xyz;
    vec3 p = pos.xyz;
    vec3 s = light.position;
    vec3 phong_color = shading_phong(light, e, p, s, n).xyz;

    //// calculate emissive color
    vec3 emissive_color = vec3(0.0,0.0,0.0);
*/
    /* your implementation starts */
    
    // Provided default implementation
    // float h = pos.z + .8;
    // h = clamp(h, 0.0, 1.0);
    // emissive_color = mix(vec3(.4,.6,.2), vec3(.4,.3,.2), h);

    const Light light = Light(
        vec3(2.5, -1.0, 4.0),
        vec3(0.28, 0.22, 0.18),
        vec3(1.6, 1.35, 1.05),
        vec3(1.0, 0.8, 0.55)
    );

    vec3 n = compute_normal(pos.xy, 0.01);
    vec3 phong_color = shading_phong(light, position.xyz, pos, light.position, n).xyz;

    float h = clamp((pos.z + 0.20) / 1.8, 0.0, 1.0);
    float slope = clamp(1.0 - dot(n, vec3(0.0, 0.0, 1.0)), 0.0, 1.0);
    float rough = 0.5 + 0.5 * noise_octave(pos.xy * 12.0, 4);

    vec3 ash = vec3(0.065, 0.055, 0.050);
    vec3 rock = vec3(0.220, 0.190, 0.160);
    vec3 basalt = vec3(0.100, 0.095, 0.090);
    vec3 hot_rock = vec3(0.340, 0.220, 0.130);
    vec3 crust = vec3(0.025, 0.020, 0.018);

    vec3 material = mix(ash, rock, smoothstep(0.0, 0.55, h));
    material = mix(material, basalt, smoothstep(0.25, 0.85, slope));
    material = mix(material, hot_rock, 0.25 * smoothstep(0.55, 1.0, h));
    material *= 0.85 + 0.22 * rough;

    float lava = volcano_lava(pos.xy);
    material = mix(material, crust, 0.45 * smoothstep(0.70, 1.0, h) * (1.0 - lava));

    vec3 lit_rock = phong_color * material;

    float pulse = 0.5 + 0.5 * sin(2.2 + pos.x * 13.0 + pos.y * 17.0);
    vec3 lava_color = mix(vec3(1.0, 0.18, 0.015), vec3(1.0, 0.82, 0.08), pulse);

    vec3 color = mix(lit_rock, lava_color, lava);
    color += lava * vec3(0.55, 0.10, 0.02);

	return color;

	// emissive_color = mix(vec3(.4,.6,.2), vec3(.4,.3,.2), h);

	/* your implementation ends */

	// return phong_color * emissive_color;
}
