
#include "VecCluster.hxx"

VecCluster::VecCluster(INT size, INT number) {
    this->size = size;
    this->number = number;
    this->cluster.resize(size * number);
}

void VecCluster::SetSize(INT size, INT number) {
    this->size = size;
    this->number = number;
    this->cluster.resize(size * number);
}

INT VecCluster::GetSize() {
    return this->size;
}

INT VecCluster::GetNumber() {
    return this->number;ｚ
}

void VecCluster::Get(INT j, VEC &v) {
    for (INT k = 0; k < this->size; ++k)
        v[k] = cluster[k + this->size * j];
}

void VecCluster::Set(INT j, VEC &v) {
    for (INT k = 0; k < this->size; ++k)
        cluster[k + this->size * j] = v[k];
}