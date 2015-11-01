#ifndef _RENDER_H_
#define _RENDER_H_

#include "vector3d.h"
#include "constant.h"
#include "hitpoint.h"
#include "ray.h"
#include "scene.h"
#include "radiance.h"
#include "random.h"
#include "ppm.h"

namespace Intersection
{
    int render(const char* filename, const int width, const int height, const int samplePerSubpixel, const int subpixel)
    {
        // レンダーした結果を格納する
        // RGBの3構成になるのでVector3d型を流用
        Vector3d *image = new Vector3d[height * width];
        
        // カメラ位置
        const Vector3d cameraPosition = Vector3d(7.0, 3.0, 7.0);
        const Vector3d cameraLookat   = Vector3d(0.0, 1.0, 0.0);
        const Vector3d cameraDir      = normalize(cameraLookat - cameraPosition);
        const Vector3d cameraUp       = Vector3d(0.0, 1.0, 0.0);

        // ワールド座標系でのイメージセンサーの大きさ
        const double sensorWidth  = 30.0 * width / height; // アスヒの調整
        const double sensorHeight = 30.0;
        // イメージセンサーまでの距離
        const double sensorDist = 45.0;
        // イメージセンサーを張るベクトル
        const Vector3d sensorVectorX = normalize(cross(cameraDir, cameraUp)) * sensorWidth;
        const Vector3d sensorVectorY = normalize(cross(sensorVectorX, cameraDir)) * sensorHeight;
        const Vector3d sensorCenter  = cameraPosition + cameraDir * sensorDist;

        for(int y = 0 ; y < height ; ++y)
        {
            for(int x = 0 ; x < width ; ++x)
            {
                const int index = (height - y - 1) * width + x;
                Random random(y * width + x + 1);

                // subpixel * subpixelのサンプリングをする
                for (int sy = 0; sy = subpixel; ++sy)
                {
                    Vector3d accumulatedRadiance = Vector3d();
                    // subpixelのサンプリング
                    for (int sx = 0; sx = subpixel; ++sx)
                    {
                        const double rate = (1.0 / subpixel);
                        const double r1 = sx * rate + rate / 2.0;
                        const double r2 = sy * rate + rate / 2.0;
                        // イメージセンサー上の位置
                        const Vector3d OnSensorPosition = sensorCenter + sensorVectorX *((r1 + x) / width - 0.5) + sensorVectorY * ((r2 + y) / height - 0.5);
                        // レイを飛ばす方向ベクトルを決定する
                        const Vector3d rayDirection = normalize(OnSensorPosition - cameraPosition);
                        accumulatedRadiance = accumulatedRadiance + radiance(Ray(cameraPosition, rayDirection), random, 0) / static_cast<double>(samplePerSubpixel) / static_cast<double>(subpixel * subpixel);
                    }
                    image[index] = image[index] + accumulatedRadiance;
                }
            }
        }
        saveImage(filename, image, width, height);
        return 0;
    }
    
    
};


#endif // _RENDER_H_