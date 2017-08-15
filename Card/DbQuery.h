#pragma once

#include <string>
#include <map>
#include <vector>

#if 0
namespace db
{
	class Dict;

	typedef std::map<std::string, std::string>	StringMap;
	typedef std::map<std::string, Dict> DictMap;

	const std::string empty = std::string();
	const Dict emptyDict;

	/*
		name
		seq (insertBefore)
	*/
	const std::string CREATE_SECTION = "create_section";


	/*
		name | sectionId
	*/
	const std::string DELETE_SECTION = "delete_section";


	/*
		none
	*/
	const std::string GET_SECTION_LIST = "get_section_list";

	/*
		oldName
		newName
	*/
	const std::string RENAME_SECTION = "rename_section";

	/*
		src  => seq:sectionId (String Map)
		seq : insertBefore
	*/
	const std::string REORDER_SECTION = "reorder_section";

	/*
	name
	seq (insertBefore)
	*/
	const std::string CREATE_TAG = "create_tag";


	/*
	name | sectionId
	*/
	const std::string DELETE_TAG = "delete_tag";


	/*
	none
	*/
	const std::string GET_TAG_LIST = "get_tag_list";

	/*
	oldName
	newName
	*/
	const std::string RENAME_TAG = "rename_tag";

	/*
	src  => seq:sectionId (String Map)
	seq : insertBefore
	*/
	const std::string REORDER_TAG = "reorder_tag";




	/*
	int			mask;

	int			pageId;
	int			seq;
	int			sectionId;
	int			level;
	CString		title;
	CString		content;
	CString		titleText;
	CString		contentText;
	CUIntArray	tags;
	*/




	// ex.  id2 : sectionId, id1 : seqId
	struct IdMap
	{
		int			id1;
		int			id2;

		bool operator< (const IdMap &idmap) const
		{
			return id1 < idmap.id1;
		}
	};

	typedef std::vector<db::IdMap>		IdMapList;
	typedef std::set<db::IdMap>			IdMapSet;

	class Dict
	{
	public:
		Dict()
		{

		}
		virtual ~Dict()
		{
		}

		int getInt(const std::string& key, int defaultVal = 0)
		{
			auto it = m_dict.find(key);
			if (it == m_dict.end())
				return defaultVal;
			
			return atoi(it->second.c_str());
		}
		const std::string & get(const std::string &key, const std::string &defaultVal = db::empty)
		{
			auto it = m_dict.find(key);
			if (it == m_dict.end())
				return defaultVal;

			return it->second;
		}
		const Dict & getDict(const std::string &key)
		{
			auto it = m_dict.find(key);
			if (it == m_dict.end())
				return emptyDict;

			std::string		key2 = "__" + key + "__";
			auto it2 = m_dictMap.find(key2);
			if (it2 == m_dictMap.end())
				return emptyDict;

			return it2->second;
		}

		void set(const std::string &key, int value)
		{
			m_dict[key] = std::to_string(value);
		}
		void set(const std::string &key, const std::string & value)
		{
			m_dict[key] = value;
		}
		void set(const std::string &key, Dict &dict)
		{
			std::string		key2 = "__" + key + "__";

			m_dict[key] = key2;
			m_dictMap[key2] = dict;
		}

		bool keyExist(const std::string &key)
		{
			return m_dict.find(key) != m_dict.end();
		}

		int size()
		{
			return m_dict.size();
		}
		
		void clear()
		{
			m_dict.clear();
		}

		db::StringMap &dict()
		{
			return m_dict;
		}

	protected:
		db::StringMap	m_dict;
		db::DictMap		m_dictMap;
	};

	class Request : public Dict
	{
	public:
		const std::string& cmd()
		{
			return m_cmd;
		}

		void setCmd(const std::string& cmd)
		{
			m_cmd = cmd;
		}

		bool verifyCmd(const std::string &cmd)
		{
			return cmd == m_cmd;
		}

	protected:
		std::string			m_cmd;
	};

	class Result
	{
	public:
		Result() : m_error(0), m_size(0) {}
		virtual ~Result() {};

		const std::string & desc()
		{
			return m_desc;
		}
		void setDesc(const std::string& desc)
		{
			m_desc = desc;
		}

		int error()
		{
			return m_error;
		}
		void setError(int error)
		{
			m_error = error;
		}
		const std::vector<Dict> &data()
		{
			return m_data;
		}
		void push_back(Dict &dict)
		{
			m_data.push_back(dict);
		}
		int size()
		{
			return m_data.size();
		}
		void clear()
		{
			m_data.clear();
			m_size = 0;
		}
	protected:
		int					m_error;
		std::string			m_desc;
		int					m_size;
		std::vector<Dict>	m_data;
	};

	class Success : public Result
	{
	public:
		Success()
		{
			m_desc = "Success";
		}
	};

	class Error : public Result
	{
	public:
		Error(int err = -1, const std::string desc = "Error")
		{
			m_desc = desc;
			m_error = err;
		}
	};

	class SqlError : public Result
	{
	public:
		SqlError(int err = -1, const std::string desc = "Sql Error")
		{
			m_desc = desc;
			m_error = err;
		}
	};

}

#endif
