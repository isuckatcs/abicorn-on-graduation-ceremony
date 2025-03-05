class NonFinal {};

class StayNonFinal {};

class Final final { virtual void foo(); };

class StayFinal final {};

struct NonFinalStruct {};

struct FinalStruct final { virtual ~FinalStruct(); };

template<typename T>
class TemplateFinal final { virtual void foo(); };

template<typename T>
class TemplateNonFinal {};

class FinalNoVirtualMethods final { void foo(); };

class FinalDtor { virtual ~FinalDtor() final; };

