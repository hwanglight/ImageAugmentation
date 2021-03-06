#ifndef MAIN_GAUSSIANNOISE_HPP
#define MAIN_GAUSSIANNOISE_HPP

#include "algo.h"
#include <QVariant>
#include <exception>

/* Algorithm of Gaussian normally distributed image noise with two modes:
 *  color and black and white noise */
class GaussianNoise : public Algorithm {
public:
    friend class GaussianNoiseBuilder;
    QImage processImage(const QImage* workingModel);
private:
    /* Degree of noise, varies from 0 to 100 */
    int32_t degreeOfNoise;
    /* Mode: white and black noise or colored */
    bool mono;
    GaussianNoise() = default;
};

/* A builder class that implements the phased construction of an algorithm object */
class GaussianNoiseBuilder {
private:
    /* Degree of noise and mode:
     * by default (before filling by the user - empty type) */
    QVariant degreeOfNoise, mono;
    /* Default options that are used only if the user has not set a value */
    const int32_t DEFAULT_degreeOfNoise = 40;
    /* By default the mode is white and black */
    const bool DEFAULT_mono = false;
public:
    GaussianNoiseBuilder() = default;
    /* Methods that set the fields of the class, for convenience,
     *  return an object of the class builder, there is no parameter check in them */
    GaussianNoiseBuilder setDegreeOfNoise(int degreeOfNoise);
    GaussianNoiseBuilder setMono(bool mono);
    /* A method in which the entered values ​​are checked
     * and exceptions are thrown in case of incorrect input,
     * the builder fields are set to the algorithm object and this object is returned */
    GaussianNoise build();
};

#endif //MAIN_GAUSSIANNOISE_HPP
