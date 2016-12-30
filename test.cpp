
#define GET_PATTERN_AND(TYPE, PATTERN, COUNT, GET1, GET2)     \
	if (GET_PATTERN(TYPE, PATTERN, COUNT, GET1, GET2) != nullptr)

#define GET_PATTERN_OR(TYPE, PATTERN, COUNT, GET1, GET2)      \
	if (GET_PATTERN(TYPE, PATTERN, COUNT, GET1, GET2) == nullptr)

#define GET_PATTERN_OR_FAIL(TYPE, PATTERN, COUNT, GET1, GET2) \
	GET_PATTERN_OR(TYPE, PATTERN, COUNT, GET1, GET2) {        \
		LOG_ERROR("Unable to find pattern " #PATTERN);        \
		return false;                                         \
	}

#define GET_PATTERN(TYPE, PATTERN, COUNT, GET1, GET2)         \
    (TYPE##_location = PATTERN.count(COUNT).get(GET1).get<TYPE>(GET2))



	auto registrationTablePattern =		pattern("76 61 49 8B 7A 40 48 8D 0D");
	auto gameStatePattern =				pattern("83 3D ? ? ? ? ? 8A D9 74 0A");
	auto getScriptIdBlock =				pattern("80 78 32 00 75 34 B1 01 E8");
	auto gameLogosPattern =				pattern("70 6C 61 74 66 6F 72 6D 3A");
	auto gameLegalsPattern =			pattern("72 1F E8 ? ? ? ? 8B 0D");
	auto onlineModelCheckBypassPtn =	pattern("48 85 C0 0F 84 ? ? ? ? 8B 48 50");
	// better onlineModelSpawnBypassPtn pattern("48 8b c8 ff 52 30 84 c0 90 90 48 8b c3"); // sfinktah
	auto onlineModelSpawnBypassPtn =	pattern("48 8B C8 FF 52 30 84 C0 74 05 48");
	auto worldPointerPattern =			pattern("48 8B 05 ? ? ? ? 45 ? ? ? ? 48 8B 48 08 48 85 C9 74 07");
	auto blipListPattern =				pattern("4C 8D 05 ? ? ? ? 0F B7 C1");
	auto globalTablePattern =			pattern("4C 8D 05 ? ? ? ? 4D 8B 08 4D 85 C9 74 11");
	
	void * void_location = nullptr;
	char * char_location = nullptr;


  // Get GlobalBasePtr
    GET_PATTERN_AND(char, globalTablePattern, 1, 0, 0) {
        globalTable.GlobalBasePtr = (__int64**)(char_location + (int)(char_location + 3) + 7);
        LOG_DEBUG("GlobalBasePtr\t 0x%p (0x%.8X)", globalTable.GlobalBasePtr, globalTable.GlobalBasePtr - get_base());
    }    else LOG_ERROR("Unable to find GlobalBasePtr");

    // Get world pointer
    GET_PATTERN_AND(char, worldPointerPattern, 1, 0, 0) {
        dwWorldPtr = reinterpret_cast<uint64_t>(char_location) + reinterpret_cast<int>(reinterpret_cast<uint64_t>(char_location) + 3) + 7;
        LOG_DEBUG("dwWorldPtr\t\t 0x%p (0x%.8X)", dwWorldPtr, dwWorldPtr - get_base());
    }    else LOG_ERROR("Unable to find world pointer");

    GET_PATTERN_AND(char, blipListPattern, 1, 0, 0) 
    {
        g_blipList = (BlipList*)(char_location + reinterpret_cast<int>(char_location + 3) + 7);
        LOG_DEBUG("blipList\t\t 0x%p (0x%.8X)", g_blipList, reinterpret_cast<uintptr_t>(g_blipList) - Memory::get_base());
    }    else LOG_ERROR("Unable to find BlipList");

    // Bypass online model requests block
    GET_PATTERN_AND(void, onlineModelCheckBypassPtn, 1, 0, 0)  Memory::nop(void_location, 24);

    // Bypass is player model allowed to spawn checks
    GET_PATTERN_AND(void, onlineModelSpawnBypassPtn, 1, 0, 8)  Memory::nop(void_location, 2);
