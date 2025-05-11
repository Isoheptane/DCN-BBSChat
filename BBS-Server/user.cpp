#include "User.h"

User::User(string name, string password) : name(name), password(password) {

}

void User::push_direct_message(string sender, Message message) {
	std::lock_guard<std::mutex> guard(this->dm_pending_mutex);
	dm_pending.emplace(sender, std::deque<Message>());
	dm_pending[sender].push_back(message);
}

std::vector<Message> User::fetch_direct_message(string sender) {
	std::lock_guard<std::mutex> guard(this->dm_pending_mutex);
	dm_pending.emplace(sender, std::deque<Message>());
	std::vector<Message> messages;

	while (!dm_pending[sender].empty()) {
		messages.push_back(dm_pending[sender].front());
		dm_pending[sender].pop_front();
	}

	return messages;
}

std::map<string, size_t> User::get_direct_message_counts() {
	std::lock_guard<std::mutex> guard(this->dm_pending_mutex);

	std::map<string, size_t> counts;
	for (auto it : dm_pending) {
		if (it.second.empty())
			continue;
		counts[it.first] = it.second.size();
	}

	return counts;
}