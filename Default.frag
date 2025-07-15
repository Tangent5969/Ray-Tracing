#version 330 core
out vec4 FragColor;

uniform vec3 camPos;
uniform mat4 model;
uniform vec2 res;
uniform float focus;

struct Ray {
	vec3 origin;
	vec3 dir;
	vec3 color;
};

struct Sphere {
	vec3 pos;
	float radius;
	vec3 color;
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
	
	
	Sphere sphere1;
	sphere1.pos = vec3(0, 0, 3);
	sphere1.radius = 0.6;
	sphere1.color = vec3(0.5, 0.2, 0.8);
	
	Ray ray;
	ray.color = vec3(0.0, 0.0, 0.0);
	ray.origin = camPos;
	ray.dir = normalize(vec3(x, y, focus));
	ray.dir = (model * vec4(ray.dir, 1.0)).xyz;
	
	if (intersect(ray, sphere1))	
		ray.color = sphere1.color;


	FragColor = vec4(ray.color, 1.0);
	//FragColor = vec4(ray.dir, 1);
};