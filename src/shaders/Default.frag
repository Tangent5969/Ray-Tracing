#version 420 core
#define maxSpheres 100
#define maxMaterials 100
#define maxRays 5
#define maxBounces 10

out vec4 FragColor;

uniform vec3 camPos;
uniform mat4 model;
uniform vec2 res;
uniform float focus;
uniform int spheresLength;


struct Ray {
	vec3 origin;
	vec3 dir;
};

struct Material {
	vec3 color; // 12 
	float lightStrength; //4
	vec3 lightColor; // 12
	float pad; // 4
};

struct Sphere {
	vec3 pos; // 12
	float radius; // 4
	Material mat; // 48
};

struct hitData {
	bool didHit;
	float dist;
	vec3 pos;
	vec3 normal;
	Material mat;
};


layout(std140) uniform objects {
	Sphere spheres[maxSpheres];
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
    float angle = 2 * 3.1415926 * random(seed);
    float value = sqrt(-2 * log(random(seed)));
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
		hit.pos = ray.origin + (ray.dir * dist1);
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
				result.mat = spheres[i].mat;
			}
		}
	}
	return result;
};


vec3 randBounce(vec3 normal, inout uint seed) {
	vec3 bounce = vec3(1);
	bounce.x = randomNormalDist(seed);
	bounce.y = randomNormalDist(seed);
	bounce.z = randomNormalDist(seed);

	// same hemisphere
	return normalize(bounce) * sign(dot(normal, bounce));
};


Ray reflectRay(vec3 hit, vec3 dir, vec3 normal) {
	Ray ray;
	ray.origin = hit;
	ray.dir = normalize(dir - 2 * dot(normal, dir) * normal);
	return ray;
};


vec3 trace(Ray ray, inout uint seed) {
	vec3 color = vec3(1.0);
	vec3 light = vec3(0.0);
	hitData hit;

	for (int i = 0; i <= maxBounces; i++) {
		hit = getCollision(ray);
		if (hit.didHit) {
			ray.origin = hit.pos;
			ray.dir = normalize(hit.normal + randBounce(hit.normal, seed));
			//ray = reflectRay(hit.pos, ray.dir, hit.normal);

			Material mat = hit.mat;
			vec3 emmited = mat.lightColor * mat.lightStrength;
			light += emmited * color;
			color *= mat.color;
	
		}
		else break;
	}
	return light;
};
	

void main() {
	uint seed = uint((gl_FragCoord.y * res.x) + gl_FragCoord.x);

	float x = -res.x / 2 + gl_FragCoord.x;
	float y = -res.y / 2 + gl_FragCoord.y;
	
	Ray ray;
	ray.origin = camPos;
	ray.dir = normalize(vec3(x, y, focus));
	ray.dir = (model * vec4(ray.dir, 1.0)).xyz;
	
	vec3 totalLight = vec3(0);

	for (int i = 0; i < maxRays; i++) {
		totalLight += trace(ray, seed);
	}
	totalLight /= maxRays;

	
	FragColor = vec4(totalLight, 1.0);

	//FragColor = vec4(ray.dir, 1.0);
		
};