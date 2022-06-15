
#include <libThirdWrap_RapidJson/json.h>

namespace rapid {

	static DNode	g_NodeNull202007301401;

	json::json(void)
	{

	}

	json::~json(void)
	{

	}

	int json::Load(const char* c_szData)
	{
		_Doc.Parse<rapidjson::kParseDefaultFlags>(c_szData);
		return !_Doc.HasParseError() ? 1 : -1 * _Doc.GetParseError();
	}

	int  json::LoadFile(const char* szFilePath)
	{
		_file_::CMyFile File;
		if (File.Open(szFilePath, "rb") == -1)
			return -1;

		_string_t s;
		s.resize(File.GetLength());
		File.Read(&s[0], File.GetLength());
		File.Close();

		return this->Load(s.c_str());
	}

	DNode& json::root(void)
	{
		return _Doc;
	}

	_string_t json::GetText(const DNode& Node)
	{
		if(Node.IsString())
			return Node.GetString();
		return "";
	}

	bool  json::SetText(const _string_t& Name, const _string_t& Text)
	{
		DNode& Node = FindNode(Name);
		if (!Node.IsNull() && Node.IsString())
			return !(Node.SetString(Text.c_str(), Text.size()).IsNull());
		return false;
	}

	bool  json::SetText(DNode& Node, const _string_t& Text)
	{
		if (Node.IsString())
			return !(Node.SetString(Text.c_str(), Text.size()).IsNull());
		return false;
	}

	bool  json::GetInt(const DNode& Node, Int64& Value)
	{
		bool isRet = true;
		if (Node.IsBool())
		{
			Value = Node.GetBool() ? 1 : 0;
		}
		else if (Node.IsInt())
		{
			Value = (Int64)Node.GetInt();
		}
		else if (Node.IsInt64())
		{
			Value = Node.GetInt64();
		}
		if (Node.IsUint())
		{
			Value = (Int64)Node.GetUint();
		}
		else if (Node.IsUint64())
		{
			Value = (Int64)Node.GetUint64();
		}
		else
			isRet = false;

		return isRet;
	}

	bool  json::SetInt(const _string_t& Name, Int64 Value)
	{
		bool isRet = false;
		DNode& Node = FindNode(Name);
		return SetInt(Node, Value);
	}

	bool  json::SetInt(DNode& Node, Int64 Value)
	{
		bool isRet = false;
		if (!Node.IsNull())
		{
			if (Node.IsInt())
			{
				isRet = !(Node.SetInt(Value).IsNull());
			}
			else if (Node.IsUint())
			{
				isRet = !(Node.SetUint(Value).IsNull());
			}
			else if (Node.IsInt64())
			{
				isRet = !(Node.SetInt64(Value).IsNull());
			}
			else if (Node.IsUint64())
			{
				isRet = !(Node.SetUint64(Value).IsNull());
			}
		}

		return isRet;
	}

	bool  json::GetDouble(const DNode& Node, double& Value)
	{
		bool isRet = true;
		if (Node.IsBool())
		{
			Value = Node.GetBool() ? 1.00 : 0;
		}
		else if (Node.IsInt())
		{
			Value = Node.GetInt();
		}
		else if (Node.IsInt64())
		{
			Value = Node.GetInt64();
		}
		if (Node.IsUint())
		{
			Value = Node.GetUint();
		}
		else if (Node.IsUint64())
		{
			Value = Node.GetUint64();
		}
		else if (Node.IsDouble())
		{
			Value = Node.GetDouble();
		}
		else
			isRet = false;

		return isRet;
	}

	bool  json::SetDouble(const _string_t& Name, double Value)
	{
		bool isRet = false;
		DNode& Node = FindNode(Name);
		return SetDouble(Node, Value);
	}

	bool  json::SetDouble(DNode& Node, double Value)
	{
		bool isRet = false;
		if (!Node.IsNull())
		{
			if (Node.IsDouble())
			{
				isRet = !(Node.SetDouble(Value).IsNull());
			}
		}

		return isRet;
	}

	bool  json::SetBool(const _string_t& Name, bool Value)
	{
		bool isRet = false;
		DNode& Node = FindNode(Name);
		return SetBool(Node, Value);
	}

	bool  json::SetBool(DNode& Node, bool Value)
	{
		bool isRet = false;
		if (!Node.IsNull())
		{
			if (Node.IsBool())
			{
				isRet = !(Node.SetBool(Value).IsNull());
			}
		}

		return isRet;
	}

	DNode&	json::FindNode(const _string_t& NodeName)
	{
		return FindNode(NodeName, root());
	}

	DNode&	json::FindNode(const _string_t& NodeName, const DNode& Start)
	{
		for (DNode::MemberIterator itr = const_cast<DNode&>(Start).MemberBegin(); itr != Start.MemberEnd(); itr++)
		{
			DNode& Key = itr->name;
			DNode& val = itr->value;
			if (val.IsObject())
			{//对象
				if (NodeName == Key.GetString())
					return val;

				DNode& Ret = FindNode(NodeName, val);
				if (!Ret.IsNull())
					return Ret;

			}
			else if (val.IsArray())
			{//数组
				for (int i = 0; i < val.Size(); i++)
				{
					DNode& Ret = FindNode(NodeName, val[i]);
					if (!Ret.IsNull())
						return Ret;
				}
			}
			else if (val.IsString())
			{//字符内容
				if (NodeName == Key.GetString())
					return val;
			}
		}

		return g_NodeNull202007301401;
	}

	DNode&  json::FindNode(const _string_t& ParentName, const _string_t& NodeName)
	{
		DNode& Ret = FindNode(ParentName);
		if (!Ret.IsNull())
		{
			return FindNode(NodeName, Ret);
		}

		return g_NodeNull202007301401;
	}

	DNode&  json::FindNode(const _string_t& ParentName, const _string_t& NodeName, const DNode& Start)
	{
		DNode& Node = FindNode(ParentName, Start);
		if (!Node.IsNull())
		{
			return FindNode(NodeName, Node);
		}

		return g_NodeNull202007301401;
	}

	DNode&  json::GetChildNode(const DNode& Node)
	{
		if (Node.IsArray())
			return const_cast<DNode&>(Node)[0];
		else if (Node.IsObject())
		{
			return const_cast<DNode&>(Node).MemberBegin()->value;
		}

		return g_NodeNull202007301401;
	}

	DNode&  json::InsertNode(const _string_t& ParentName,
		const _string_t& NodeName,
		const _string_t& NodeText)
	{
		DNode& Node = FindNode(ParentName);
		if (!Node.IsNull())
		{
			return InsertNode(Node, NodeName, NodeText);
		}

		return g_NodeNull202007301401;
	}

	DNode&  json::InsertNode(const DNode& Parent,
		const _string_t& NodeName,
		const _string_t& NodeText)
	{
		if (!Parent.IsNull())
		{
			if (Parent.IsObject())
			{
				return const_cast<DNode&>(Parent).AddMember(DNode().SetString(NodeName.c_str(), AllocatorInstance()),
					DNode().SetString(NodeText.c_str(), AllocatorInstance()), AllocatorInstance());
			}
			else if (Parent.IsArray())
			{
				DNode value;// (rapidjson::kObjectType);
				value.AddMember(DNode().SetString(NodeName.c_str(), AllocatorInstance()),
					DNode().SetString(NodeText.c_str(), AllocatorInstance()), AllocatorInstance());
				return const_cast<DNode&>(Parent).PushBack(value, AllocatorInstance());
			}
		}

		return g_NodeNull202007301401;
	}

	DNode&  json::InsertNode(const _string_t& ParentName,
		const _string_t& NodeName, Int64 Value)
	{
		DNode& Node = FindNode(ParentName);
		if (!Node.IsNull())
		{
			return InsertNode(Node, NodeName, Value);
		}

		return g_NodeNull202007301401;
	}

	DNode&  json::InsertNode(const DNode& Parent,
		const _string_t& NodeName, Int64 Value)
	{
		if (!Parent.IsNull())
		{
			if (Parent.IsObject())
			{
				return const_cast<DNode&>(Parent).AddMember(DNode().SetString(NodeName.c_str(), AllocatorInstance()),
					DNode().SetInt64(Value), AllocatorInstance());
			}
			else if (Parent.IsArray())
			{
				DNode value;// (rapidjson::kObjectType);
				value.AddMember(DNode().SetString(NodeName.c_str(), AllocatorInstance()),
					DNode().SetInt64(Value), AllocatorInstance());
				return const_cast<DNode&>(Parent).PushBack(value, AllocatorInstance());
			}
		}

		return g_NodeNull202007301401;
	}

	DNode&  json::InsertNode(const _string_t& ParentName,
		const _string_t& NodeName, double Value)
	{
		DNode& Node = FindNode(ParentName);
		if (!Node.IsNull())
		{
			return InsertNode(Node, NodeName, Value);
		}

		return g_NodeNull202007301401;
	}

	DNode&  json::InsertNode(const DNode& Parent,
		const _string_t& NodeName, double Value)
	{
		if (!Parent.IsNull())
		{
			if (Parent.IsObject())
			{
				return const_cast<DNode&>(Parent).AddMember(DNode().SetString(NodeName.c_str(), AllocatorInstance()),
					DNode().SetDouble(Value), AllocatorInstance());
			}
			else if (Parent.IsArray())
			{
				DNode value;// (rapidjson::kObjectType);
				value.AddMember(DNode().SetString(NodeName.c_str(), AllocatorInstance()),
					DNode().SetDouble(Value), AllocatorInstance());
				return const_cast<DNode&>(Parent).PushBack(value, AllocatorInstance());
			}
		}

		return g_NodeNull202007301401;
	}

	DNode&  json::InsertNode(const _string_t& ParentName,
		const _string_t& NodeName,
		bool Value)
	{
		DNode& Node = FindNode(ParentName);
		if (!Node.IsNull())
		{
			return InsertNode(Node, NodeName, Value);
		}

		return g_NodeNull202007301401;
	}

	DNode&  json::InsertNode(const DNode& Parent,
		const _string_t& NodeName,
		bool Value)
	{
		if (!Parent.IsNull())
		{
			if (Parent.IsObject())
			{
				return const_cast<DNode&>(Parent).AddMember(DNode().SetString(NodeName.c_str(), AllocatorInstance()),
					DNode().SetBool(Value), AllocatorInstance());
			}
			else if (Parent.IsArray())
			{
				DNode value;// (rapidjson::kObjectType);
				value.AddMember(DNode().SetString(NodeName.c_str(), AllocatorInstance()),
					DNode().SetBool(Value), AllocatorInstance());
				return const_cast<DNode&>(Parent).PushBack(value, AllocatorInstance());
			}
		}

		return g_NodeNull202007301401;
	}

	DNode&  json::InsertNode(const DNode& Parent,
		const _string_t& NodeName,
		const DNode& Node)
	{
		if (!Parent.IsNull() && !Node.IsNull())
		{
			if (Parent.IsObject())
			{
				return const_cast<DNode&>(Parent).AddMember(DNode().SetString(NodeName.c_str(), AllocatorInstance()),
					const_cast<DNode&>(Node), AllocatorInstance());
			}
			else if (Parent.IsArray())
			{
				DNode value(rapidjson::kObjectType);
				value.AddMember(DNode().SetString(NodeName.c_str(), AllocatorInstance()), const_cast<DNode&>(Node), AllocatorInstance());
				return const_cast<DNode&>(Parent).PushBack(value, AllocatorInstance());
			}
		}

		return g_NodeNull202007301401;
	}

	DNode&  json::InsertNode(const _string_t& ParentName, const DNode& Node)
	{
		DNode& tNode = FindNode(ParentName);
		return InsertNode(tNode, Node);
	}

	DNode&  json::InsertNode(const DNode& Parent, const DNode& Node)
	{
		if (!Parent.IsNull() && !Node.IsNull())
		{
			if (Parent.IsArray())
			{
				return const_cast<DNode&>(Parent).PushBack(const_cast<DNode&>(Node), AllocatorInstance());
			}
		}

		return g_NodeNull202007301401;
	}

	bool	json::RemoveNode(const _string_t &ParentName, const _string_t &NodeName)
	{
		DNode& Parent = FindNode(ParentName);
		if (!Parent.IsNull())
		{
			return Parent.RemoveMember(NodeName.c_str());
		}

		return false;
	}

	bool  json::RemoveNode(DNode& Node)
	{
		if (!Node.IsNull())
		{
			return root().RemoveMember(Node);
		}

		return true;
	}

	DNode&  json::InsertObject(const _string_t& ParentName, const _string_t& sName, const DObject& Obj)
	{
		DNode&	Node = FindNode(ParentName);
		return InsertNode(Node, sName, Obj.Get());
	}

	DNode&  json::InsertObject(const DNode& Parent, const _string_t& sName, const DObject& Obj)
	{
		return InsertNode(Parent, sName, Obj.Get());
	}

	DNode&  json::InsertArray(const _string_t& ParentName, const _string_t& sName, const DArray& Array)
	{
		DNode&	Node = FindNode(ParentName);
		return InsertNode(Node, sName, Array.Get());
	}

	DNode&  json::InsertArray(const DNode& Parent, const _string_t& sName, const DArray& Array)
	{
		return InsertNode(Parent, sName, Array.Get());
	}

	bool json::SaveFile(const _string_t& Path)
	{
		_file_::CMyFile File;
		if (File.Open(Path.c_str(), "wb") == -1)
			return false;

		_string_big_t s = GetString();
		File.Write(s.c_str(), s.size());
		File.Close();
		return true;
	}

	_string_big_t json::GetString(void)
	{
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		_Doc.Accept(writer);
		return buffer.GetString();
	}
}