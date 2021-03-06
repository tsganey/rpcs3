#include "stdafx.h"
#include "Utilities/rXml.h"
#include "Emu/System.h"
#include "TROPUSR.h"

bool TROPUSRLoader::Load(const std::string& filepath, const std::string& configpath)
{
	const std::string& path = vfs::get(filepath);

	if (!fs::is_file(path))
	{
		Generate(filepath, configpath);
	}

	if (!m_file.open(path, fs::read))
	{
		return false;
	}

	if (!LoadHeader() || !LoadTableHeaders() || !LoadTables())
	{
		return false;
	}

	m_file.release();
	return true;
}

bool TROPUSRLoader::LoadHeader()
{
	if (!m_file)
	{
		return false;
	}

	m_file.seek(0);

	if (!m_file.read(m_header))
	{
		return false;
	}

	return true;
}

bool TROPUSRLoader::LoadTableHeaders()
{
	if (!m_file)
	{
		return false;
	}

	m_file.seek(0x30);
	m_tableHeaders.clear();
	m_tableHeaders.resize(m_header.tables_count);

	for (TROPUSRTableHeader& tableHeader : m_tableHeaders)
	{
		if (!m_file.read(tableHeader))
			return false;
	}

	return true;
}

bool TROPUSRLoader::LoadTables()
{
	if (!m_file)
	{
		return false;
	}

	for (const TROPUSRTableHeader& tableHeader : m_tableHeaders)
	{
		m_file.seek(tableHeader.offset);

		if (tableHeader.type == 4)
		{
			m_table4.clear();
			m_table4.resize(tableHeader.entries_count);

			for (auto& entry : m_table4)
			{
				if (!m_file.read(entry))
					return false;
			}
		}

		if (tableHeader.type == 6)
		{
			m_table6.clear();
			m_table6.resize(tableHeader.entries_count);

			for (auto& entry : m_table6)
			{
				if (!m_file.read(entry))
					return false;
			}
		}

		// TODO: Other tables
	}

	return true;
}

// TODO: TROPUSRLoader::Save deletes the TROPUSR and creates it again. This is probably very slow.
bool TROPUSRLoader::Save(const std::string& filepath)
{
	if (!m_file.open(vfs::get(filepath), fs::rewrite))
	{
		return false;
	}

	m_file.write(m_header);

	for (const TROPUSRTableHeader& tableHeader : m_tableHeaders)
	{
		m_file.write(tableHeader);
	}

	for (const auto& entry : m_table4)
	{
		m_file.write(entry);
	}

	for (const auto& entry : m_table6)
	{
		m_file.write(entry);
	}

	m_file.release();
	return true;
}

bool TROPUSRLoader::Generate(const std::string& filepath, const std::string& configpath)
{
	const std::string& path = vfs::get(configpath);

	// TODO: rXmlDocument can open only real file
	ASSERT(!fs::get_virtual_device(path));
	rXmlDocument doc;
	doc.Load(path);

	m_table4.clear();
	m_table6.clear();

	for (std::shared_ptr<rXmlNode> n = doc.GetRoot()->GetChildren(); n; n = n->GetNext())
	{
		if (n->GetName() == "trophy")
		{
			u32 trophy_id = std::atoi(n->GetAttribute("id").c_str());
			u32 trophy_grade;
			switch (((const char *)n->GetAttribute("ttype").c_str())[0])
			{
			case 'B': trophy_grade = 4; break;
			case 'S': trophy_grade = 3; break;
			case 'G': trophy_grade = 2; break;
			case 'P': trophy_grade = 1; break;
			default: trophy_grade = 0;
			}

			TROPUSREntry4 entry4 = { 4, SIZE_32(TROPUSREntry4) - 0x10, (u32)m_table4.size(), 0, trophy_id, trophy_grade, 0xFFFFFFFF };
			TROPUSREntry6 entry6 = { 6, SIZE_32(TROPUSREntry6) - 0x10, (u32)m_table6.size(), 0, trophy_id };

			m_table4.push_back(entry4);
			m_table6.push_back(entry6);
		}
	}

	u64 offset = sizeof(TROPUSRHeader) + 2 * sizeof(TROPUSRTableHeader);
	TROPUSRTableHeader table4header = { 4, SIZE_32(TROPUSREntry4) - 0x10, 1, (u32)m_table4.size(), offset };
	offset += m_table4.size() * sizeof(TROPUSREntry4);
	TROPUSRTableHeader table6header = { 6, SIZE_32(TROPUSREntry6) - 0x10, 1, (u32)m_table6.size(), offset };
	offset += m_table6.size() * sizeof(TROPUSREntry6);

	m_tableHeaders.clear();
	m_tableHeaders.push_back(table4header);
	m_tableHeaders.push_back(table6header);

	m_header.magic = 0x818F54AD;
	m_header.unk1 = 0x00010000;
	m_header.tables_count = (u32)m_tableHeaders.size();
	m_header.unk2 = 0;

	Save(filepath);

	return true;
}

u32 TROPUSRLoader::GetTrophiesCount()
{
	return (u32)m_table6.size();
}

u32 TROPUSRLoader::GetTrophyUnlockState(u32 id)
{
	if (id >= m_table6.size())
	{
		LOG_WARNING(LOADER, "TROPUSRLoader::GetUnlockState: Invalid id=%d", id);
	}

	return m_table6[id].trophy_state; // Let's assume the trophies are stored ordered
}

u64 TROPUSRLoader::GetTrophyTimestamp(u32 id)
{
	if (id >= m_table6.size())
	{
		LOG_WARNING(LOADER, "TROPUSRLoader::GetTrophyTimestamp: Invalid id=%d", id);
	}

	// TODO: What timestamp does sceNpTrophyGetTrophyInfo want, timestamp1 or timestamp2? 
	return m_table6[id].timestamp2; // Let's assume the trophies are stored ordered
}

bool TROPUSRLoader::UnlockTrophy(u32 id, u64 timestamp1, u64 timestamp2)
{
	if (id >= m_table6.size())
	{
		return false;
	}

	m_table6[id].trophy_state = 1;
	m_table6[id].timestamp1 = timestamp1;
	m_table6[id].timestamp2 = timestamp2;

	return true;
}
