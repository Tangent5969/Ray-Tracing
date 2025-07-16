#version 420 core
#define maxSpheres 100

out vec4 FragColor;

uniform vec3 camPos;
uniform mat4 model;
uniform vec2 res;
uniform float focus;
uniform int spheresLength;

struct Ray {
	vec3 origin;
	vec3 dir;
	vec3 color;
};

struct Sphere {
	vec3 pos; // 16
	float radius; // 4
	vec3 color; // 16
};


layout(std140) uniform objects {
	Sphere spheres[maxSpheres];
};

bool intersect(Ray ray, Sphere sphere) {
	vec3 a = sphere.pos - ray.origin;
	float b = dot(ray.dir, a);
	float c = dot(a, a) - sphere.radius * sphere.radius;
	float d = b * b - c;
	float dist1, dist2;
	if (d >= 0) {
		dist1 = b + sqrt(d);
		dist2 = b - sqrt(d);
		if (dist1 >= 0 || dist2 >= 0) return true;
		return false;
	}
	return false;
};
	

void main() {
	float x = -res.x / 2 + gl_FragCoord.x;
	float y = -res.y / 2 + gl_FragCoord.y;
	
	
	Ray ray;
	ray.color = vec3(0.0, 0.2, 0.0);
	ray.origin = camPos;
	ray.dir = normalize(vec3(x, y, focus));
	ray.dir = (model * vec4(ray.dir, 1.0)).xyz;


	for (int i = 0; i < spheresLength; i++) {
		if (intersect(ray, spheres[i]))	
			ray.color = spheres[i].color;
		}


	FragColor = vec4(ray.color, 1.0);

};