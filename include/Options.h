#ifndef ABICORN_OPTIONS_H
#define ABICORN_OPTIONS_H

namespace abicorn {
struct AbicornOptions {
  int UseColor : 1;

  AbicornOptions() { UseColor = 0; }
};
} // namespace abicorn

#endif // ABICORN_OPTIONS_H
