// -----------------------------------------------------------------------------
// DEBUG
// -----------------------------------------------------------------------------

// If not on PIO or not defined in platformio.ini
#ifndef DO_DEBUG
// Debug output set to 0 to disable app debug output
#define DO_DEBUG 0
#endif

#if DO_DEBUG > 0
#define DEBUG_MSG(tag, ...)                \
	do                                   \
	{                                    \
		if (tag)                         \
        mcSerial.print("");\
		mcSerial.printf("  <%s> ", tag); \
		mcSerial.printf(__VA_ARGS__);      \
		mcSerial.printf("\n");             \
	} while (0)

#define DEBUG_MSG_VAL(tag, val, ...)                \
	do                                   \
	{                                    \
		if (tag)                         \
        mcSerial.print("");\
		mcSerial.printf("  <%s> ", tag); \
		mcSerial.printf(__VA_ARGS__);      \
		mcSerial.printf("  <%d> ", val); \
		mcSerial.printf("\n");             \
	} while (0)

#define DEBUG_MSG_TXT(tag, txt, ...)                \
	do                                   \
	{                                    \
		if (tag)                         \
        mcSerial.print("");\
		mcSerial.printf("  <%s> ", tag); \
		mcSerial.printf(__VA_ARGS__);      \
		mcSerial.printf("  %s ", txt); \
		mcSerial.printf("\n");             \
	} while (0)

#else
#define DEBUG_MSG(...)
#define DEBUG_MSG_VAL(...)
#define DEBUG_MSG_TXT(...)
#endif