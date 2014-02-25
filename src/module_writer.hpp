#ifndef H_MODULE_WRITER
#define H_MODULE_WRITER

class SlalomFunction {
public:
  SlalomFunction() {}
};

class ModuleWriter {
public:
  virtual ~ModuleWriter() {}
  virtual SlalomFunction* createICombinator() = 0;
  virtual SlalomFunction* createKCombinator() = 0;
  virtual SlalomFunction* createSCombinator() = 0;
  virtual SlalomFunction* createDerivedCombinator(const std::string&) = 0;
  virtual SlalomFunction* createApplication(SlalomFunction*, SlalomFunction*) = 0;
};

#endif
