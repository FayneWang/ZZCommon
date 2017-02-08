#pragma once

// A macro to disallow operator=
// This should be used in the private: declarations for a class.
#define ZZ_DISALLOW_ASSIGN_(type) \
    void operator=(type const &){}

// A macro to disallow copy constructor and operator=
// This should be used in the private: declarations for a class.
#define ZZ_DISALLOW_COPY_AND_ASSIGN_(type) \
	type(type const &){} \
    ZZ_DISALLOW_ASSIGN_(type)

/**
 * 声明单例模式的私有成员内容
 */
#define ZZ_SINGLETON_PRIVATE_DECLARE(type) \
		public: \
			static type * Instance(); \
			static void   Uninstance(); \
		private: \
			type(); \
			~type(); \
			ZZ_DISALLOW_COPY_AND_ASSIGN_(type); \
			static type *s_pSingleton;

#define  ZZ_SAFE_DELETE(ptr) if (ptr) \
                             {\
                                delete (ptr);\
                                (ptr) = 0;\
                             }

#define ZZ_SAFE_DELETE_ARRAY(ptr) if(ptr) \
                                  {\
                                     delete [] (ptr);\
                                     (ptr) = 0;\
                                  }

#define ZZ_SAFE_FREE(ptr) if(ptr) \
                          {\
                             free(ptr);\
                             (ptr) = 0;\
                          }

#define ZZ_SAFE_RELEASE(ptr)  if(ptr) \
                              {\
                                 (ptr)->Release();\
                                 (ptr) = 0;\
                              }

#define ZZ_IsX64Process()	(8==sizeof(void*))
