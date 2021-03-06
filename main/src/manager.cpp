#include "../include/manager.h"

void AlgorithmManager::processRequests(GlobalRequest r) {
    /* Check whether file or directory was provided */
    if (r.getLoadDirectoryOrFile().isFile()) {
        processSingleImage(r);
    } else {
        processDirectory(r);
    }
}

void AlgorithmManager::appendNewCombination(std::vector<QString>& allCombinations, int32_t depth,
                                            int32_t numberOfAlgorithms, int32_t current, QString overlay) {
    if (depth - current > numberOfAlgorithms - overlay.length()) {
        return;
    }
    if (overlay.length() == numberOfAlgorithms) {
        allCombinations.push_back(overlay);
        return;
    }
    appendNewCombination(allCombinations, depth, numberOfAlgorithms, current, overlay + "0");
    if (current < depth) {
        appendNewCombination(allCombinations, depth, numberOfAlgorithms, current + 1, overlay + "1");
    }
}

std::vector<QString> AlgorithmManager::getCombinationsOfAlgorithms(std::size_t limit,
                                                   std::vector<int32_t>& depths,
                                                   int32_t numberOfAlgorithms) {
    std::vector<QString> allPossibleAlgorithms;
    for (auto depth : depths) {
        if (depth >= 1 && depth <= numberOfAlgorithms) {
            /* Generate all possible bit strings with depth bit set */
            appendNewCombination(allPossibleAlgorithms, depth, numberOfAlgorithms, 0, "");
        }
    }
    std::random_device random;
    std::mt19937 g(random());
    /* Randomly shuffle all bit string */
    std::shuffle(allPossibleAlgorithms.begin(), allPossibleAlgorithms.end(), g);
    /* Take only first 'limit' images */
    if (allPossibleAlgorithms.size() > limit) {
        allPossibleAlgorithms.resize(limit);
    }
    return allPossibleAlgorithms;
}

void AlgorithmManager::processSingleImage(GlobalRequest request) {
    std::vector<std::pair<std::shared_ptr<QImage>, int>> images;
    QString fileName = request.getLoadDirectoryOrFile().absoluteFilePath();
    QImage image(fileName);
    /* User may provide file with the image format which in fact is not an image */
    if (image.isNull()) {
        fprintf(stderr, "%s\n", qPrintable("Error: unable to process " + fileName));
        return;
    }
    auto requests = request.getRequests();
    auto depthsOfOverlay = request.getDepthsOfOverlay();
    std::vector<QString> algoCombinations = getCombinationsOfAlgorithms(request.getLimitOfPictures(),
                                                                        depthsOfOverlay,
                                                                        requests.size());
    for (auto combination : algoCombinations) {
        QImage img(fileName);
        int32_t numberOfAlgorithmsApllied = 0;
        int algoType = -1;
        for (int i = 0; i < combination.length(); i++) {
            if (combination[i] == "1") {
                std::shared_ptr<Algorithm> ptr = f.getAlgo(requests[i]);
                img = ptr->processImage(&img);
                numberOfAlgorithmsApllied++;
                algoType = requests[i]->type;
            }
        }
        auto picture = std::make_shared<QImage>(img);
        if (numberOfAlgorithmsApllied == 1) {
            images.push_back(std::make_pair(picture, algoType));
        } else {
            images.push_back(std::make_pair(picture, combined));
        }
    }
    imgwriter.saveToDirectory(request.getSaveDirectory().absoluteFilePath(),
                              images,
                              request.getFileFormat());
}

void AlgorithmManager::processDirectory(GlobalRequest r) {
    ImageLoader imgloader;
    QDir directory = r.getLoadDirectoryOrFile().absoluteFilePath();
    std::vector<QString> filePaths = imgloader.getAbsoluteFilePaths(directory);
    for (auto path : filePaths) {
        r.setLoadDirectoryOrFile(QFileInfo(path));
        processSingleImage(r);
    }
}

QImage AlgorithmManager::preview(GlobalRequest r) {
    QString fileName = r.getLoadDirectoryOrFile().absoluteFilePath();
    QImage q(fileName);
    if (q.isNull()) {
         throw std::invalid_argument("Error: unable to process image");
    }
    for (auto request : r.getRequests()) {
        std::unique_ptr<Algorithm> ptr = f.getAlgo(request);
        q = ptr->processImage(&q);
    }
    return q;
}
