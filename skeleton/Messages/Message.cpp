#include "Message.h"
#include "../checkMemoryLeaks.h"

std::unordered_multimap<int, message::Message> message::MessageManager::_messages = std::unordered_multimap<int, message::Message>();

std::list<message::Message> message::MessageManager::receiveMessages(int id) {
	std::list<Message> tmp;
	auto it = _messages.find(id);
	while (it != _messages.end()) {
		tmp.push_back(it->second);
		it = _messages.erase(it);
		it = _messages.find(id);
	}
	return tmp;
}