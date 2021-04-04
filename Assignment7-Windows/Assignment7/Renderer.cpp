//
// Created by goksu on 2/25/20.
//

#include <fstream>
#include "Scene.hpp"
#include "Renderer.hpp"
#include <thread>
#include <string>

inline float deg2rad(const float& deg) { return deg * M_PI / 180.0; }

const float EPSILON = 0.00001;

struct ThreadData{
    std::vector<Ray> rays;
    // std::vector<Vector3f> &framebuffer;
    std::vector<int> buffer_inds;
    // const int spp;
    // const Scene& scene;
    // ThreadData(std::vector<Vector3f> framebuffer, int spp, const Scene &s) : framebuffer(framebuffer), spp(spp), scene(s){}
};

void myThread(ThreadData &task){
    
}

// The main render function. This where we iterate over all pixels in the image,
// generate primary rays and cast these rays into the scene. The content of the
// framebuffer is saved to a file.
void Renderer::Render(const Scene& scene, const int spp)
{
    std::vector<Vector3f> framebuffer(scene.width * scene.height);

    float scale = tan(deg2rad(scene.fov * 0.5));
    float imageAspectRatio = scene.width / (float)scene.height;
    Vector3f eye_pos(278, 273, -800);
    int m = 0;

    // change the spp value to change sample ammount
    // int spp = SPP;
    std::cout << "SPP: " << spp << "\n";
    const int NUM_WORKERS = this->thread_num;
    std::vector<ThreadData> worker_datas(NUM_WORKERS);
    
    for (uint32_t j = 0; j < scene.height; ++j) {
        for (uint32_t i = 0; i < scene.width; ++i) {
            // generate primary ray direction
            float x = (2 * (i + 0.5) / (float)scene.width - 1) *
                      imageAspectRatio * scale;
            float y = (1 - 2 * (j + 0.5) / (float)scene.height) * scale;
            

            Vector3f dir = normalize(Vector3f(-x, y, 1));

            int which_worker = m % NUM_WORKERS;
            worker_datas[which_worker].rays.push_back(Ray(eye_pos, dir));
            worker_datas[which_worker].buffer_inds.push_back(m); 


            // for (int k = 0; k < spp; k++){
            //     framebuffer[m] += scene.castRay(Ray(eye_pos, dir), 0) / spp;  
            // }
            m++;
        }
        // UpdateProgress(j / (float)scene.height);
    }
    // UpdateProgress(1.f);
    std::vector<std::thread> workers;
    for(int i = 0 ; i < NUM_WORKERS; ++i){
        workers.push_back(std::thread{
            [&](ThreadData &taskdata, int worker_id){
                int cnt = taskdata.rays.size();
                
                for(int i = 0; i < cnt; ++i ){
                     for (int k = 0; k < spp; k++){
                        framebuffer[taskdata.buffer_inds[i]] += scene.castRay(taskdata.rays[i], 0) / spp;  
                    }
                    if( (i+1) % 1000 == 0){
                        char buf[100];
                        sprintf(buf, "Thread %02d: %05d / %05d", worker_id, i+1 , cnt);
                        std::cout << std::string(buf)  <<std::endl;;
                    }
                }
            }
            , std::ref(worker_datas[i]), i
        });
    }
    for (auto &w: workers){
        w.join();
    }

    // save framebuffer to file
    char filename[100];
    sprintf(filename, "visible_WH_%04d_SPP_%03d_RR_%.2f.ppm", scene.width, spp, scene.P_RR);
    FILE* fp = fopen(filename, "wb");
    (void)fprintf(fp, "P6\n%d %d\n255\n", scene.width, scene.height);
    for (auto i = 0; i < scene.height * scene.width; ++i) {
        static unsigned char color[3];
        color[0] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].x), 0.6f));
        color[1] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].y), 0.6f));
        color[2] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].z), 0.6f));
        fwrite(color, 1, 3, fp);
    }
    fclose(fp);    
}
