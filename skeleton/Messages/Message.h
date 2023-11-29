#pragma once
#include <cstdint>
#include <unordered_map>
#include <list>
class ForceGenerator;
namespace message {
	enum class msgID {
		_m_GENERATOR_ERASABLE
	};

	struct Message {
		int id;

		struct {
			ForceGenerator* fg;
		} genData;

		Message(int id) : id(id) {};
		Message() = default;
	};

	class MessageManager {
	private:
		static std::unordered_multimap<int, Message> _messages;
	public:
		static inline void sendMessage(const Message& m) {_messages.insert({ m.id, m });}
		static std::list<Message> receiveMessages(int id);
	};
}