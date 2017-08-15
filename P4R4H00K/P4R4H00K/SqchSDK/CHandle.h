#pragma once

namespace SSDK {
	class CBaseHandle {
		friend class C_BaseEntityList;
	public:
		inline CBaseHandle::CBaseHandle(unsigned long value) {
			m_Index = value;
		}
	protected:
		unsigned long  m_Index;
	};
}