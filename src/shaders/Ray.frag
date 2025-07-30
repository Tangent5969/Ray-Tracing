#version 450 core

#define pi 3.1415926f

#define maxSpheres 100
#define maxMaterials 100

in vec2 texCoords;
out vec4 FragColor;

uniform int rayCount;
uniform int maxBounces;
uniform vec3 camPos;
uniform mat4 model;
uniform vec2 res;
uniform float focus;
uniform int spheresLength;
uniform sampler2D text;
uniform int accumulationFrame;
uniform bool environmentLightFlag;

struct Ray {
	vec3 origin;
	vec3 dir;
};

struct Material {
	vec3 color; // 12 
	float lightStrength; //4
	vec3 lightColor; // 12
	float smoothness; // 4
	float gloss; // 4
	bool glass; // 4
	float refraction; // 4
	float pad; // 4
};

struct Sphere {
	vec3 pos; // 12
	float radius; // 4
	int matIndex; // 4
	float pad1; // 4
	float pad2; // 4
	float pad3; // 4
};

struct hitData {
	bool didHit;
	bool inside;
	float dist;
	vec3 pos;
	vec3 normal;
	Material mat;
};


layout(std140) uniform objects {
	Sphere spheres[maxSpheres];
	Material materials[maxMaterials];
};


// https://www.shadertoy.com/view/XlGcRh
float random(inout uint seed) {
	seed = seed * 747796405 + 2891336453;
	uint result = ((seed >> ((seed >> 28) + 4)) ^ seed) * 277803737;
	result = (result >> 22) ^ result;
	return result / 4294967295.0;
}


// equal distribution across sphere
// https://dspguide.com/ch2/6.html
float randomNormalDist(inout uint seed) {
    float angle = 2.0f * pi * random(seed);
    float value = sqrt(-2.0f * log(random(seed)));
    return value * cos(angle);
}


hitData intersect(Ray ray, Sphere sphere) {
	hitData hit;
	hit.didHit = false;

	// setting up quadratic
	vec3 a = sphere.pos - ray.origin;
	float b = dot(ray.dir, a);
	float c = dot(a, a) - sphere.radius * sphere.radius;
	float d = b * b - c;

	// any real roots
	if (d >= 0.0f) {
		float dist = b - sqrt(d);
		// 0.001 sets clip distance and fixes banding
		if (dist < 0.001f) { 
			dist = b + sqrt(d);
			// missed
			if (dist < 0.001f) return hit;
		}

		// hit
		hit.dist = dist;
		hit.didHit = true;
		hit.pos = ray.origin + (ray.dir * dist);
		hit.normal = normalize(hit.pos - sphere.pos);

		// inside or outside sphere
		if (dot(ray.dir, hit.normal) > 0.0f) {
			hit.normal = -hit.normal;
			hit.inside = true;
		} else {
			hit.inside = false;
		}

	}
	return hit;
};


hitData getCollision(Ray ray) {
	hitData result;
	result.didHit = false;
	result.dist = 9999.0f;

	for (int i = 0; i < spheresLength; i++) {
		hitData hit = intersect(ray, spheres[i]);
		if (hit.didHit) {
			if (hit.dist < result.dist) {
				result = hit;
				result.mat = materials[spheres[i].matIndex];
			}
		}
	}
	return result;
};


vec3 randBounce(vec3 normal, inout uint seed) {
	vec3 bounce = vec3(1.0f);
	bounce.x = randomNormalDist(seed);
	bounce.y = randomNormalDist(seed);
	bounce.z = randomNormalDist(seed);

	// same hemisphere
	return normalize(bounce) * sign(dot(normal, bounce));
};


vec3 reflectRay(vec3 dir, vec3 normal) {
	return normalize(dir - 2.0f * dot(normal, dir) * normal);
};


vec3 environmentLight(Ray ray) {
	return vec3(0.4, 0.5, 0.9) * (ray.dir.y + 0.95);
}


// snells law, gives refracted angle
vec3 snells(vec3 dir, vec3 normal, float index, float cosine) {
	float value = 1 - index * index * (1 - cosine * cosine);
	// reflect
	if (value < 0.0f) return vec3(0);
	// refract
	return index * dir - (index * cosine - sqrt(value)) * normal;
}


// Schlicks approximation
float fresnel(float index, float cosine) {
        float r0 = (1 - index) / (1 + index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow(1 - cosine, 5);
}


vec3 trace(Ray ray, inout uint seed) {
	vec3 color = vec3(1.0f);
	vec3 light = vec3(0.0f);
	hitData hit;

	for (int i = 0; i <= maxBounces; i++) {
		hit = getCollision(ray);
		if (hit.didHit) {
			Material mat = hit.mat;

			ray.origin = hit.pos;

			// transparent
			if (mat.glass) {
				float index = hit.inside ? 1.0f / mat.refraction : mat.refraction;
				float cosine = dot(-ray.dir, hit.normal);
				vec3 refractDir = snells(ray.dir, hit.normal, index, cosine);
				if (refractDir == 0 || fresnel(index, cosine) > random(seed)) ray.dir = reflectRay(ray.dir, hit.normal);
				else ray.dir = refractDir;
				// blur
				ray.dir += (1 - mat.smoothness) * randBounce(hit.normal, seed);
			}

			// matt and reflective
			else {
				vec3 diffuseDir = randBounce(hit.normal, seed);
				// chance of diffuse or specular reflection
				if (random(seed) >= mat.gloss) ray.dir = diffuseDir;
				// specular reflection / blur
				else ray.dir = mat.smoothness * reflectRay(ray.dir, hit.normal) + (1 - mat.smoothness) * diffuseDir;
			}

			// light
			vec3 emmited = mat.lightColor * mat.lightStrength;
			light += emmited * color;
			color *= mat.color * dot(hit.normal, ray.dir);
			
		}
		else {
			if (environmentLightFlag) {
				light += environmentLight(ray) * color;
			}
			break;
		}
	}
	return light;
};
	

void main() {
	uint seed = uint((gl_FragCoord.y * res.x) + gl_FragCoord.x) + accumulationFrame * 20983476; // large arbitrary  number 
	float x = -res.x / 2.0f + gl_FragCoord.x;
	float y = -res.y / 2.0f + gl_FragCoord.y;
	
	Ray ray;
	ray.origin = camPos;
	ray.dir = normalize(vec3(x, y, focus));
	ray.dir = (model * vec4(ray.dir, 1.0f)).xyz;
	
	vec3 totalLight = vec3(0);

	for (int i = 0; i < rayCount; i++) {
		totalLight += trace(ray, seed);
	}
	totalLight /= rayCount;

	// pixel accumulation from previous frames
	float weight = 1.0f / accumulationFrame;
	totalLight = (1.0f - weight) * texture(text, texCoords).xyz + totalLight * weight;
	
	// final output color
	FragColor = vec4(totalLight, 1.0f);
};