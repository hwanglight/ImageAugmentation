#include "../include/manager.h"

void AlgorithmManager::process_requests(std::vector<Request*>& r, QString path_from, QString path_to) {
    QImage q(path_from);
    for (auto i : r) {
        std::unique_ptr<Algorithm> ptr = f.get_algo(i);
        q = ptr->processImage(&q);
    }
    q.save(path_to);
}
