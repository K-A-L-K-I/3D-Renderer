#include <SDL.h>
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <utility>
#include <iostream>

constexpr int CANVAS_WIDTH  = 600;
constexpr int CANVAS_HEIGHT = 600;
struct Vector3 {
    double x, y, z;
    Vector3(double x, double y, double z) : x(x), y(y), z(z) {}
    Vector3 operator+(const Vector3& v) const { return { x + v.x, y + v.y, z + v.z }; }
    Vector3 operator-(const Vector3& v) const { return { x - v.x, y - v.y, z - v.z }; }
    Vector3 operator*(double k) const { return { x * k, y * k, z * k }; }
};
double dot(const Vector3& a, const Vector3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}


static uint32_t framebuffer[CANVAS_WIDTH * CANVAS_HEIGHT];

void PutPixel(int x, int y, uint32_t color) {
    int sx = CANVAS_WIDTH  / 2 + x;
    int sy = CANVAS_HEIGHT / 2 - y;

    if (sx < 0 || sx >= CANVAS_WIDTH || sy < 0 || sy >= CANVAS_HEIGHT)
        return;

    framebuffer[sy * CANVAS_WIDTH + sx] = color;
}

uint32_t MakeColor(uint8_t r, uint8_t g, uint8_t b) {
    return (0xFF << 24) | (r << 16) | (g << 8) | b;
}

void ClearCanvas(uint32_t color) {
    for (int i = 0; i < CANVAS_WIDTH * CANVAS_HEIGHT; i++)
        framebuffer[i] = color;
}

struct Sphere {
    Vector3 center;
    double radius;
    uint32_t color;
    Sphere(Vector3 c, double r, uint32_t col) : center(c), radius(r), color(col) {}
};

Sphere spheres[] = {
    Sphere(Vector3(0, -1, 3),  1, MakeColor(255, 0, 0)),
    Sphere(Vector3(2,  0, 4),  1, MakeColor(0, 0, 255)),
    Sphere(Vector3(-2, 0, 4),  1, MakeColor(0, 255, 0)),
};
int numSpheres = 3;

constexpr double Vw = 1.0;
constexpr double Vh = 1.0;
constexpr double d  = 1.0;

Vector3 CanvasToViewport(int x,int y) {
    return { x * Vw / CANVAS_WIDTH, y * Vh / CANVAS_HEIGHT, d};
}
std::pair<double, double> IntersectRaySphere(Vector3 O, Vector3 D, Sphere sphere)
{

    Vector3 CO = O - sphere.center;
    double a = dot(D, D);
    double b = 2 * dot(CO, D);
    double c = dot(CO, CO) - sphere.radius * sphere.radius;
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return { 1e18, 1e18 };
    }
    else {
        double t1 = (-b + sqrt(discriminant)) / (2 * a);
        double t2 = (-b - sqrt(discriminant)) / (2 * a);
        return { t1, t2 };
    }
}
uint32_t TraceRay(Vector3 O, Vector3 D, double t_min, double t_max) {
    double closest_t = 1e18;
    int closest_sphere_index = -1;
    for (int i = 0; i <= numSpheres - 1; i++) {
        auto [t1, t2] = IntersectRaySphere(O, D, spheres[i]);
        if (t1 >= t_min && t1 <= t_max && t1 < closest_t) {
            closest_t = t1;
            closest_sphere_index = i;
        }
        if (t2 >= t_min && t2 <= t_max && t2 < closest_t) {
            closest_t = t2;
            closest_sphere_index = i;
        }
    }
    if (closest_sphere_index == -1) {
        return MakeColor(255, 255, 255);
    }
    else {
        return spheres[closest_sphere_index].color;
    }
}
int main(int argc, char* argv[]) {

    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "CG from Scratch",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        CANVAS_WIDTH, CANVAS_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        CANVAS_WIDTH, CANVAS_HEIGHT
    );

    ClearCanvas(MakeColor(15, 15, 15));
    Vector3 O(0, 0, 0);
    for (int x = -CANVAS_WIDTH / 2; x < CANVAS_WIDTH / 2; x++)
    {
        for (int y = -CANVAS_HEIGHT / 2; y < CANVAS_HEIGHT / 2; y++) {
            Vector3 D = CanvasToViewport(x, y);
            uint32_t color = TraceRay(O, D, 1, 1e18);
            PutPixel(x, y, color);
        }
            

    }
        

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                running = false;
        }

        SDL_UpdateTexture(texture, nullptr, framebuffer, CANVAS_WIDTH * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
