# Task 4.1

+ Для синхронизации доступа к volatile переменным не нужно использовать дополнительных барьеров памяти,
  в mutex_barrier_weak используем std::memory_order_relaxed.
+ Для синхронизации доступа к обычным переменным необходимо запретить компилятору перемещать операции наружу
  или вовнутрь критической секции, поэтому используем std::memory_order_acquire и std::memory_order_release.

##Test result:
hardware_concurrency : 4
limit : 100000

nthreads :1
    my futex time : 130ms
  std::mutex time : 2ms
mutex strong time : 147ms
  mutex weak time : 161ms
nthreads :2
    my futex time : 205ms
  std::mutex time : 52ms
mutex strong time : 178ms
  mutex weak time : 222ms
nthreads :3
    my futex time : 148ms
  std::mutex time : 229ms
mutex strong time : 236ms
  mutex weak time : 152ms
nthreads :4
    my futex time : 285ms
  std::mutex time : 471ms
mutex strong time : 136ms
  mutex weak time : 139ms
nthreads :5
    my futex time : 269ms
  std::mutex time : 496ms
mutex strong time : 242ms
  mutex weak time : 246ms
nthreads :6
    my futex time : 243ms
  std::mutex time : 351ms
mutex strong time : 242ms
  mutex weak time : 247ms
nthreads :7
    my futex time : 245ms
  std::mutex time : 364ms
mutex strong time : 244ms
  mutex weak time : 250ms
nthreads :8
    my futex time : 243ms
  std::mutex time : 367ms
mutex strong time : 242ms
  mutex weak time : 249ms
nthreads :9
    my futex time : 283ms
  std::mutex time : 538ms
mutex strong time : 251ms
  mutex weak time : 284ms
nthreads :10
    my futex time : 265ms
  std::mutex time : 474ms
mutex strong time : 245ms
  mutex weak time : 281ms
