#pragma once

struct CProbeObject
{
	int id;
	std::string name;
	struct less {
		constexpr bool operator()(const CProbeObject& lhs, const CProbeObject& rhs) const {
			return lhs.id < rhs.id;
		}
	};
	struct hash {
		std::size_t operator()(const CProbeObject& o) noexcept {
			return std::hash<int>{}(o.id);
		}
	};
};

