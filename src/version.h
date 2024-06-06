/*
 ============================================================================
 Name       :  version.h
 Date       :  27-09-2023
 Author     :  Lukasz Wisniewski
 Email      :  lukasz.wisniew@gmail.com
 Copyright  :  (C) 2023 Lukasz Wisniewski
 ============================================================================
*/



#ifndef VERSION_H_
#define VERSION_H_

/** Commit number */
#ifndef GIT_COMMIT
 #define GIT_COMMIT "0"
#endif

/** Program version */
#ifndef VERSION
 #define VERSION "0.1.01"
#endif

/** Release date of program */
#ifndef RELEASE_DATE
 #define RELEASE_DATE "06.06.2024"
#endif

/** Program compilation date */
#ifndef BUILD_DATE
 #define BUILD_DATE "06.06.2024"
#endif

/** Program description */
#ifndef PROGRAM_DOC
 #define PROGRAM_DOC "DB Connection worker."
#endif

#endif /* VERSION_H_ */
