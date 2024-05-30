#include "util.hh"

#include <math.h>
#include <string.h>
#include <time.h>

void free_nothing(__attribute__((unused)) void *_) {}

char not_strcmp(void *a, void *b) {
  return !(char)strcmp((char *)a, (char *)b);
}

void sleep_nano(uint64_t n) {
  struct timespec rem;
  rem.tv_nsec = n % 1000000000;
  rem.tv_sec = n / 1000000000;

  int r;
  r = nanosleep(&rem, &rem);
  while (r) {
    r = nanosleep(&rem, &rem);
  }
}

Vec2 Vec2::operator-(Vec2 b) { return {this->x - b.x, this->y - b.y}; }
Vec2 Vec2::operator+(Vec2 b) { return {this->x + b.x, this->y + b.y}; }
Vec2 Vec2::operator*(float a) { return {a * this->x, a * this->y}; }
float Vec2::operator*(Vec2 b) { return this->x * b.x + this->y * b.y; }
Vec2 Vec2::operator/(float b) { return {this->x / b, this->y / b}; }
float Vec2::angle() { return atan2f(this->x, this->y); }
float Vec2::len() { return sqrtf(this->x * this->x + this->y * this->y); }
Vec2 Vec2::truncate(float b) {
  return this->len() > b ? this->normalize() * b : *this;
}
Vec2 Vec2::normalize() {
  return {this->x / this->len(), this->y / this->len()};
}
