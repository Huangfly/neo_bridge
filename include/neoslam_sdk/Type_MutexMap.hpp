//
// Created by huang on 18-4-14.
//

#ifndef NEO_BRIDGE_QUEUE_HPP
#define NEO_BRIDGE_QUEUE_HPP



#include <stdint.h>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <map>
#include <utility>


namespace Neo{

    template <class T1, class T2> class MutexMap {
        public:
            MutexMap():max_size(1000){}
            MutexMap(int max = 1000) : max_size(max) {}

            // Empty the queue
            void clear() {
                std::unique_lock<std::mutex> lock(the_mutex);
                while (!the_map.empty()) {
                    the_map.clear();
                }
            }

            // Add an element to the queue.
            void inset(const std::pair<const T1, T2> &_pair) {
                std::lock_guard<std::mutex> lock(the_mutex);
                the_map.insert(_pair);
                the_cond_var.notify_one();
            }

            bool set(const T1 key, const T2 val){
                std::unique_lock<std::mutex> lock(the_mutex);


                if(the_map.find(key) != the_map.end()){
                    the_map[key] = val;
                    return true;
                }
                return false;

            }


            // If the queue is empty, wait till an element is avaiable.
            bool findKey(const T1 _k) {
                std::unique_lock<std::mutex> lock(the_mutex);

                if(the_map.find(_k) != the_map.end()){
                    return true;
                }
                return false;
            }

            bool get(const T1 _k, T2 &_v) {
                std::unique_lock<std::mutex> lock(the_mutex);

                if(the_map.find(_k) != the_map.end()){
                    _v = the_map[_k];
                    return true;
                }
                return false;
            }

            bool get(const T1 _k, T2 **_v) {
                std::unique_lock<std::mutex> lock(the_mutex);

                if(the_map.find(_k) != the_map.end()){
                    *_v = &the_map[_k];
                    return true;
                }
                return false;
            }

            bool erase(const T1 _k){
                std::unique_lock<std::mutex> lock(the_mutex);
                if(the_map.find(_k) == the_map.end()){
                    return false;
                }
                the_map.erase(_k);
                return true;
            }

            int size(){

                if(the_map.empty()) return 0;

                return (int)the_map.size();
            }

            typename std::map<T1, T2>::iterator begin(){
                return the_map.begin();
            }

            typename std::map<T1,T2>::iterator end(){
                return the_map.end();
            };

        private:
            int max_size;
            std::map<T1,T2> the_map;
            mutable std::mutex the_mutex;
            mutable std::condition_variable the_cond_var;
        };

} // ns neo

#endif //NEO_BRIDGE_QUEUE_HPP
