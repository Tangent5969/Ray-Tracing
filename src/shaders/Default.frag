#version 420 core
#define maxSpheres 100
#define maxMaterials 100
#define maxBounces 1

out vec4 FragColor;

uniform vec3 camPos;
uniform mat4 model;
uniform vec2 res;
uniform float focus;
uniform int spheresLength;
uniform uint seed;
uint newSeed = seed;


struct Ray {
	vec3 origin;
	vec3 dir;
};

struct material {
	vec3 color; // 12
	float pad1; // 4
	vec3 lightStrength; // 12
	float pad2; // 4
	vec3 lightColor; // 12
	float pad3; // 4
};


struct Sphere {
	vec3 pos; // 12
	float radius; // 4
	vec3 color; // 12
	float pad; // 4

};

struct hitData {
	bool didHit;
	float dist;
	vec3 pos;
	vec3 normal;
	vec3 color;
};


layout(std140) uniform objects {
	Sphere spheres[maxSpheres];
};

// https://www.shadertoy.com/view/XlGcRh
float random(inout uint state) {
	state = state * 747796405 + 2891336453;
	uint result = ((state >> ((state >> 28) + 4)) ^ state) * 277803737;
	result = (result >> 22) ^ result;
	return result / 4294967295.0; // 2^32 - 1
}


hitData intersect(Ray ray, Sphere sphere) {
	hitData hit;
	hit.didHit = false;

	// setting up quadratic
	vec3 a = sphere.pos - ray.origin;
	float b = dot(ray.dir, a);
	float c = dot(a, a) - sphere.radius * sphere.radius;
	float d = b * b - c;
	float dist1, dist2;

	if (d >= 0) {
		dist1 = b + sqrt(d);
		dist2 = b - sqrt(d);
		if (dist1 > dist2) {
			// swap (smallest value in dist1)
			float temp = dist1;
			dist1 = dist2;
			dist2 = temp;
		}
		if (dist1 < 0) {
			dist1 = dist2;
			// missed
			if (dist1 < 0) return hit;
		}
		// hit
		hit.dist = dist1;
		hit.didHit = true;
		hit.pos = ray.origin + ray.dir * dist1;
		hit.normal = normalize(hit.pos - sphere.pos);
	}
	return hit;
};


hitData getCollision(Ray ray) {
	hitData result;
	result.dist = 9999;

	for (int i = 0; i < spheresLength; i++) {
		hitData hit = intersect(ray, spheres[i]);
		if (hit.didHit) {
			if (hit.dist < result.dist) {
				result = hit;
				result.color = spheres[i].color;
			}
		}
	}
	return result;
};

vec3 randBounce(vec3 normal) {
	vec3 bounce = vec3(1);
	bounce.x = random(newSeed) * 2 - 1;
	bounce.y = random(newSeed) * 2 - 1;
	bounce.z = random(newSeed) * 2 - 1;

	return normalize(bounce) * sign(dot(normal, bounce));
}
	

void main() {
	
	float x = -res.x / 2 + gl_FragCoord.x;
	float y = -res.y / 2 + gl_FragCoord.y;
	
	Ray ray;
	ray.origin = camPos;
	ray.dir = normalize(vec3(x, y, focus));
	ray.dir = (model * vec4(ray.dir, 1.0)).xyz;

	vec3 color = vec3(1.0);

	for (int i = 0; i < maxBounces; i++) {
		hitData hit = getCollision(ray);
		if (hit.didHit) {
			color *= hit.color;
			ray.origin = hit.pos;
			ray.dir = randBounce(hit.normal);
		}
		else break;
	}

	
	FragColor = vec4(color, 1.0);

	//FragColor = vec4(ray.dir, 1.0);
		
};