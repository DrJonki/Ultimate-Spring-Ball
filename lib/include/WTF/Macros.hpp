#define WTF_NO_COPY(className)                \
  className(const className&) = delete;       \
  void operator =(const className&) = delete