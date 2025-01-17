#ifndef  PIKA_MONITOR_THREAD_H_
#define  PIKA_MONITOR_THREAD_H_
#include <map>
#include <set>
#include <atomic>
#include <list>
#include <deque>
#include <queue>

#include "pink_thread.h"
#include "pink_epoll.h"
#include "pika_client_conn.h"
#include "slash_mutex.h"
#include "pika_define.h"

class PikaMonitorThread : public pink::Thread {
public:
  PikaMonitorThread();
  virtual ~PikaMonitorThread();

  void AddMonitorClient(pink::RedisConn* client_ptr);
  void AddMonitorMessage(std::string monitor_message);
  int32_t ThreadClientList(std::vector<std::pair<int32_t, std::string> >* client = NULL);
  bool ThreadClientKill(const std::string& ip_port = "all");
  bool HasMonitorClients();

private:
  void AddCronTask(MonitorCronTask task);
  bool FindClient(const std::string& ip_port);
  pink::WriteStatus SendMessage(int32_t fd, std::string& message);
  void RemoveMonitorClient(const std::string& ip_port);

  slash::Mutex monitor_mutex_protector_;
  slash::CondVar monitor_cond_;

  std::list<std::pair<int32_t, std::string> > monitor_clients_;
  std::deque<std::string> monitor_messages_;
  std::atomic<bool> is_running_;
  std::atomic<bool> should_exit_;
  std::queue<MonitorCronTask> cron_tasks_;

  virtual void* ThreadMain();
  void RemoveMonitorClient(int32_t client_fd);
  void StartThread();
};
#endif
