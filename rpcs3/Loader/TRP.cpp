#include "stdafx.h"
#include "Emu/System.h"
#include "TRP.h"

TRPLoader::TRPLoader(const fs::file& f)
	: trp_f(f)
{
}

bool TRPLoader::Install(const std::string& dest, bool show)
{
	if (!trp_f)
	{
		return false;
	}

	const std::string& local_path = vfs::get(dest);

	if (!fs::create_dir(local_path) && fs::error != EEXIST)
	{
		return false;
	}

	std::vector<char> buffer; buffer.reserve(65536);

	for (const TRPEntry& entry : m_entries)
	{
		trp_f.seek(entry.offset);
		buffer.resize(entry.size);
		if (!trp_f.read(buffer)) continue; // ???
		fs::file(local_path + '/' + entry.name, fs::rewrite).write(buffer);
	}

	return true;
}

bool TRPLoader::LoadHeader(bool show)
{
	if (!trp_f)
	{
		return false;
	}

	trp_f.seek(0);

	if (!trp_f.read(m_header))
	{
		return false;
	}

	if (m_header.trp_magic != 0xDCA24D00)
	{
		return false;
	}

	if (show)
	{
		LOG_NOTICE(LOADER, "TRP version: 0x%x", m_header.trp_version);
	}

	m_entries.clear();
	m_entries.resize(m_header.trp_files_count);

	for (u32 i = 0; i < m_header.trp_files_count; i++)
	{
		if (!trp_f.read(m_entries[i]))
		{
			return false;
		}

		if (show)
		{
			LOG_NOTICE(LOADER, "TRP entry #%d: %s", m_entries[i].name);
		}
	}

	return true;
}

bool TRPLoader::ContainsEntry(const char *filename)
{
	for (const TRPEntry& entry : m_entries)
	{
		if (!strcmp(entry.name, filename))
		{
			return true;
		}
	}
	return false;
}

void TRPLoader::RemoveEntry(const char *filename)
{
	std::vector<TRPEntry>::iterator i = m_entries.begin();
	while (i != m_entries.end())
	{
		if (!strcmp(i->name, filename))
		{
			i = m_entries.erase(i);
		}
		else
		{
			i++;
		}
	}
}

void TRPLoader::RenameEntry(const char *oldname, const char *newname)
{
	for (const TRPEntry& entry : m_entries)
	{
		if (!strcmp(entry.name, oldname))
		{
			memcpy((void*)entry.name, newname, 32);
		}
	}
}
