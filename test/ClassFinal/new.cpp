class NonFinal final {};
// CHECK: new.cpp{{:[0-9]*:[0-9]*:}} warning: change in the finality of record 'NonFinal' [class-checker]
// CHECK-NEXT: class NonFinal final {};
// CHECK-NEXT:       ^
// CHECK-NEXT: old.cpp{{:[0-9]*:[0-9]*:}} note: record 'NonFinal' is declared as 'non-final' in the old library [class-checker]
// CHECK-NEXT:class NonFinal {};
// CHECK-NEXT:      ^
// CHECK-NEXT: new.cpp{{:[0-9]*:[0-9]*:}} note: record 'NonFinal' is declared as 'final' in the new library [class-checker]    
// CHECK-NEXT:class NonFinal final {};
// CHECK-NEXT:      ^

class StayNonFinal {};

class Final { virtual void foo(); };
// CHECK-NEXT: new.cpp{{:[0-9]*:[0-9]*:}} warning: change in the finality of record 'Final' [class-checker]
// CHECK-NEXT: class Final { virtual void foo(); };
// CHECK-NEXT:       ^
// CHECK-NEXT: old.cpp{{:[0-9]*:[0-9]*:}} note: record 'Final' is declared as 'final' in the old library [class-checker]       
// CHECK-NEXT: class Final final { virtual void foo(); };
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp{{:[0-9]*:[0-9]*:}} note: record 'Final' is declared as 'non-final' in the new library [class-checker]   
// CHECK-NEXT: class Final { virtual void foo(); };
// CHECK-NEXT:       ^

class StayFinal final {};

struct NonFinalStruct final {};
// CHECK-NEXT: new.cpp{{:[0-9]*:[0-9]*:}} warning: change in the finality of record 'NonFinalStruct' [class-checker]
// CHECK-NEXT: struct NonFinalStruct final {};
// CHECK-NEXT:        ^
// CHECK-NEXT: old.cpp{{:[0-9]*:[0-9]*:}} note: record 'NonFinalStruct' is declared as 'non-final' in the old library [class-checker]
// CHECK-NEXT: struct NonFinalStruct {};
// CHECK-NEXT:        ^
// CHECK-NEXT: new.cpp{{:[0-9]*:[0-9]*:}} note: record 'NonFinalStruct' is declared as 'final' in the new library [class-checker]
// CHECK-NEXT: struct NonFinalStruct final {};
// CHECK-NEXT:        ^

struct FinalStruct { virtual ~FinalStruct(); };
// CHECK-NEXT: new.cpp{{:[0-9]*:[0-9]*:}} warning: change in the finality of record 'FinalStruct' [class-checker]
// CHECK-NEXT: struct FinalStruct { virtual ~FinalStruct(); };
// CHECK-NEXT:        ^
// CHECK-NEXT: old.cpp{{:[0-9]*:[0-9]*:}} note: record 'FinalStruct' is declared as 'final' in the old library [class-checker]
// CHECK-NEXT: struct FinalStruct final { virtual ~FinalStruct(); };
// CHECK-NEXT:        ^
// CHECK-NEXT: new.cpp{{:[0-9]*:[0-9]*:}} note: record 'FinalStruct' is declared as 'non-final' in the new library [class-checker]
// CHECK-NEXT: struct FinalStruct { virtual ~FinalStruct(); };
// CHECK-NEXT:        ^

template<typename T>
class TemplateFinal { virtual void foo(); };
// CHECK-NEXT: new.cpp{{:[0-9]*:[0-9]*:}} warning: change in the finality of record 'TemplateFinal' [class-checker]
// CHECK-NEXT: class TemplateFinal { virtual void foo(); };
// CHECK-NEXT:       ^
// CHECK-NEXT: old.cpp{{:[0-9]*:[0-9]*:}} note: record 'TemplateFinal' is declared as 'final' in the old library [class-checker]
// CHECK-NEXT: class TemplateFinal final { virtual void foo(); };
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp{{:[0-9]*:[0-9]*:}} note: record 'TemplateFinal' is declared as 'non-final' in the new library [class-checker]
// CHECK-NEXT: class TemplateFinal { virtual void foo(); };
// CHECK-NEXT:       ^

template<typename T>
class TemplateNonFinal final {};
// CHECK-NEXT: new.cpp{{:[0-9]*:[0-9]*:}} warning: change in the finality of record 'TemplateNonFinal' [class-checker]
// CHECK-NEXT: class TemplateNonFinal final {};
// CHECK-NEXT:       ^
// CHECK-NEXT: old.cpp{{:[0-9]*:[0-9]*:}} note: record 'TemplateNonFinal' is declared as 'non-final' in the old library [class-checker]
// CHECK-NEXT: class TemplateNonFinal {};
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp{{:[0-9]*:[0-9]*:}} note: record 'TemplateNonFinal' is declared as 'final' in the new library [class-checker]
// CHECK-NEXT: class TemplateNonFinal final {};
// CHECK-NEXT:       ^

class FinalNoVirtualMethods { void foo(); };

class FinalDtor final { virtual ~FinalDtor() final; };
// CHECK-NOT: {{.*}}
