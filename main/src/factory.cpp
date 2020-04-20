#include "../include/factory.h"
#include "../include/algo.h"
#include "../include/rotate90.h"
#include "../include/verticalflip.h"
#include "../include/horizontalflip.h"
#include "../include/cropfrommiddle.h"
#include "../include/dithering.h"
#include "../include/gaussiannoise.h"
#include "../include/kuwahara.h"
#include "../include/lightening.h"
#include "../include/whiteblack.h"
#include <memory>
#include <cassert>

//to do: process wrong arguments
std::unique_ptr<Algorithm> Factory::get_algo(std::shared_ptr<Request> r) {
    if (r->type == crop) {
        CropGivenPieceBuilder builder;
        std::shared_ptr<CropRequest> ptr = std::static_pointer_cast<CropRequest> (r);
        if (ptr->x != -INT_MAX) {
            builder.setUpperLeftXInPercent(ptr->x);
        }
        if (ptr->x != -INT_MAX) {
            builder.setUpperLeftYInPercent(ptr->y);
        }
        if (ptr->x != -INT_MAX) {
            builder.setDownRightXInPercent(ptr->cols);
        }
        if (ptr->x != -INT_MAX) {
            builder.setDownRightYInPercent(ptr->rows);
        }
        return std::make_unique<CropGivenPiece>(builder.build());
    }
    if (r->type == gaussiannoise) {
        GaussianNoiseBuilder builder;
        std::shared_ptr<GaussianNoiseRequest> ptr =
                std::static_pointer_cast<GaussianNoiseRequest> (r);
        builder.setDegreeOfNoise(ptr->degreeOfNoise);
        builder.setMono(ptr->mono);
        return std::make_unique<GaussianNoise>(builder.build());
    }
    if (r->type == dithering) {
        DitheringBuilder builder;
        return std::make_unique<Dithering>(builder.build());
    }
    if (r->type == kuwahara) {
        KuwaharaBuilder builder;
        std::shared_ptr<KuwaharaRequest> ptr =
                std::static_pointer_cast<KuwaharaRequest> (r);
        builder.setDegreeOfBlur(ptr->degreeOfBlur);
        return std::make_unique<Kuwahara>(builder.build());
    }
    if (r->type == light) {
        LighteningBuilder builder;
        std::shared_ptr<LighteningRequest> ptr =
                std::static_pointer_cast<LighteningRequest> (r);
        builder.setdDegreeOfLightening(ptr->degreeOfLightening);
        return std::make_unique<Lightening>(builder.build());
    }
    if (r->type == rgb) {
        RGBToneBuilder builder;
        std::shared_ptr<RGBToneRequest> ptr =
                std::static_pointer_cast<RGBToneRequest> (r);
        builder.setdDegreeOfTone(ptr->degreeOfTone);
        builder.setColor(ptr->color);
        return std::make_unique<RGBTone>(builder.build());
    }
    if (r->type == hflip) {
        HorizontalFlipBuilder builder;
        return std::make_unique<HorizontalFlip>(builder.build());
    }
    if (r->type == whiteblack) {
        WhiteBlackBuilder builder;
        return std::make_unique<WhiteBlack>(builder.build());
    }
    if (r->type == vflip) {
        VerticalFlipBuilder builder;
        return std::make_unique<VerticalFlip>(builder.build());
    }
    if (r->type == rotate90) {
        Rotate90Builder builder;
        return std::make_unique<Rotate90>(builder.build());
    }
    if (r->type == rotate45) {
        Rotate45Builder builder;
        return std::make_unique<Rotate45>(builder.build());
    }
}
