#include "M_FileSystem.h"

#include <physfs.h>
#include <SDL_filesystem.h>


M_FileSystem::M_FileSystem() : Module("M_FileSystem", true)
{
	LOG_CREATION(moduleName.c_str());

	char* basePath = SDL_GetBasePath();
	LOG(CONSOLE_TEXT_BLUE, "FS bsae path: [%s].", basePath);
	PHYSFS_init(basePath);

	AddPath(".");
	AddPath("Data");

	SDL_free(basePath);

	configuration = M_INIT | M_CLEAN_UP | M_SAVE_CONFIG;
}


M_FileSystem::~M_FileSystem()
{
	LOG_DESTRUCTION(moduleName.c_str());

	PHYSFS_deinit();
}

bool M_FileSystem::Init()
{
	LOG_INIT(moduleName.c_str());

	bool ret = true;

	char* writePath = SDL_GetBasePath();

	if (PHYSFS_setWriteDir(writePath) == 0)
	{
		LOG(LOG_ERROR, "Could not set write dir: %s\n", writePath);
		ret = false;
	}
	else
	{
		LOG(CONSOLE_TEXT_BLUE, "FS write path: [%s].", writePath);
		AddPath(writePath);
	}

	SDL_free(writePath);

	return ret;
}

bool M_FileSystem::CleanUp()
{
	LOG_CLEANUP(moduleName.c_str());

	return true;
}

bool M_FileSystem::AddPath(const char * pathOrZip, const char * mountPoint)
{
	bool ret = false;

	if (PHYSFS_mount(pathOrZip, mountPoint, 1) == 0)
	{
		LOG(LOG_ERROR, "Error while adding a path or zip(%s): %s.", pathOrZip, PHYSFS_getLastError());
	}
	else
	{
		ret = true;
	}

	return ret;
}

bool M_FileSystem::Exist(const char * file) const
{
	return PHYSFS_exists(file);
}

bool M_FileSystem::MakeDir(const char * path, const char * mount) const
{
	bool ret = false;

	if (path)
	{
		char newDir[128];

		if (mount) sprintf_s(newDir, 128, "%s%s", mount, path);
		else sprintf_s(newDir, 128, path);

		if (PHYSFS_mkdir(newDir) == 0)
		{
			LOG(LOG_WARN, "Could not create dir: %s. Error: %s.", newDir, PHYSFS_getLastError());
		}
		else
		{
			LOG(LOG_INFO, "Just created new dir: %s.", newDir);
			ret = true;
		}
	}

	return ret;
}

bool M_FileSystem::IsDirectory(const char * path) const
{
	return PHYSFS_isDirectory(path);
}

const char * M_FileSystem::GetBaseDir() const
{
	return PHYSFS_getBaseDir();
}

void M_FileSystem::DisplaySearchPaths() const
{
	for (char** i = PHYSFS_getSearchPath(); *i != NULL; ++i)
		LOG(LOG_INFO, "[%s] is in search path.", *i);
}

int M_FileSystem::GetSearchPaths(std::vector<std::string>& paths)
{
	for (char** i = PHYSFS_getSearchPath(); *i != NULL; ++i)
		paths.push_back(*i);

	return paths.size();
}

uint M_FileSystem::LoadFile(const char * file, char ** buffer) const
{
	uint ret = 0;

	PHYSFS_file* fsFile = PHYSFS_openRead(file);

	if (fsFile)
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(fsFile);

		if (size > 0)
		{
			*buffer = new char[(uint)size];
			PHYSFS_sint64 readed = PHYSFS_read(fsFile, *buffer, 1, (PHYSFS_sint32)size);
			if (readed != size)
			{
				LOG(LOG_WARN, "Error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				RELEASE(buffer);
			}
			else
			{
				ret = (uint)readed;
			}
		}

		if (PHYSFS_close(fsFile) == 0)
			LOG(LOG_WARN, "Error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
	{
		LOG(LOG_WARN, "Error while opening file %s: %s\n", file, PHYSFS_getLastError());
	}

	return ret;
}

uint M_FileSystem::SaveFile(const char * file, const void * buffer, uint size) const
{
	uint ret = 0;

	PHYSFS_file* fsFile = PHYSFS_openWrite(file);

	if (fsFile)
	{
		PHYSFS_sint64 written = PHYSFS_write(fsFile, (const void*)buffer, 1, size);
		if (written != size)
		{
			LOG(LOG_ERROR, "Error while writing to file %s: %s\n", file, PHYSFS_getLastError());
		}
		else
		{
			ret = (uint)written;
		}

		if (PHYSFS_close(fsFile) == 0)
			LOG(LOG_ERROR, "Error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
	{
		LOG(LOG_ERROR, "Error while opening file %s: %s\n", file, PHYSFS_getLastError());
	}

	return ret;
}

uint M_FileSystem::GetFilesOnDir(const char * dir, std::vector<std::string>& files) const
{
	uint ret = 0;

	char** rc = PHYSFS_enumerateFiles(dir);
	for (char** it = rc; *it != nullptr; ++it)
	{
		files.push_back(*it);
		++ret;
	}

	PHYSFS_freeList(rc);

	return ret;
}

uint M_FileSystem::GetFilesAndDirs(const char * dir, std::vector<std::string>& files, std::vector<std::string>& dirs) const
{
	uint ret = 0;

	std::string directory(dir);

	char** rc = PHYSFS_enumerateFiles(dir);
	for (char** it = rc; *it != nullptr; ++it)
	{
		if (IsDirectory((directory + *it).c_str()))
			dirs.push_back(*it);
		else
			files.push_back(*it);
		++ret;
	}

	PHYSFS_freeList(rc);

	return ret;
}

double M_FileSystem::GetLastModification(const char * file) const
{
	return PHYSFS_getLastModTime(file);
}

bool M_FileSystem::DestroyFile(std::string filename) const
{
	int ret;
	std::vector<std::string> directories, files;
	if (IsDirectory(filename.c_str()))
	{
		GetFilesAndDirs(filename.c_str(), files, directories);
		for (std::vector<std::string>::const_iterator directory = directories.begin(); directory != directories.end(); ++directory)
		{
			std::string directory_name = filename + *directory + "/";
			DestroyFile(directory_name);
		}

		for (std::vector<std::string>::const_iterator file = files.begin(); file != files.end(); ++file)
			PHYSFS_delete((filename + '/' + (*file)).data());
	}
	ret = PHYSFS_delete(filename.data());

	return (ret) ? true : false;
}

bool M_FileSystem::DuplicateFile(const char * src, const char * dst) const
{
	bool ret = false;

	if (!src || !dst)return ret;

	char* buffer = nullptr;
	int size = LoadFile(src, &buffer);

	if (size > 0)
	{
		int succes = SaveFile(dst, buffer, size);
		LOG(LOG_INFO, "Fs: Duplicated file from src [%s] to dst [%s].", src, dst);
		ret = (succes == size);
	}
	else
	{
		LOG(LOG_ERROR, "Could not duplicate file from src [%s] to dst [%s].", src, dst);
	}

	RELEASE_ARRAY(buffer);

	return ret;
}
