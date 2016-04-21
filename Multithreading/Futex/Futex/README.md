# Task 4.1

+ Для синхронизации доступа к volatile переменным не нужно использовать дополнительных барьеров памяти,
  в mutex_barrier_weak используем std::memory_order_relaxed.
+ Для синхронизации доступа к обычным переменным необходимо запретить компилятору перемещать операции наружу
  или вовнутрь критической секции, поэтому используем std::memory_order_acquire и std::memory_order_release.

##Test result:

hardware_concurrency : 4

limit : 100000

+ nthreads :1

        my futex time : 143ms

      std::mutex time : 2ms

    mutex strong time : 136ms

      mutex weak time : 141ms

+ nthreads :2

        my futex time : 97ms

      std::mutex time : 404ms

    mutex strong time : 128ms

      mutex weak time : 136ms

+ nthreads :3

        my futex time : 141ms

      std::mutex time : 339ms

    mutex strong time : 146ms

      mutex weak time : 162ms

+ nthreads :4

        my futex time : 218ms

      std::mutex time : 400ms

    mutex strong time : 181ms

      mutex weak time : 225ms

+ nthreads :5

        my futex time : 250ms

      std::mutex time : 362ms

    mutex strong time : 267ms

      mutex weak time : 257ms

+ nthreads :6

        my futex time : 258ms

      std::mutex time : 362ms

    mutex strong time : 243ms

      mutex weak time : 249ms

+ nthreads :7

        my futex time : 251ms

      std::mutex time : 418ms

    mutex strong time : 247ms

      mutex weak time : 253ms

+ nthreads :8

        my futex time : 247ms

      std::mutex time : 363ms

    mutex strong time : 244ms

      mutex weak time : 248ms

+ nthreads :9

        my futex time : 247ms

      std::mutex time : 363ms

    mutex strong time : 245ms

      mutex weak time : 250ms

+ nthreads :10

        my futex time : 248ms

      std::mutex time : 363ms

    mutex strong time : 246ms

      mutex weak time : 251ms
