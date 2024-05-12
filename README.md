# Операционные системы ИДЗ-2 

Самилык Анастасия БПИ229

## Вариант 28
## Работа выполнена на оценку 10.

### Индивидуальное условие: Задача о производстве булавок

В цехе по заточке булавок все необходимые операции осуществляются рабочими на трех производственных участках.

1. На первом участке 1 ≤ K ≤ 3 рабочих получают тупые булавки и в течение некоторого случайного времени каждый из них проверяет ее на предмет кривизны. Если булавка не кривая, то рабочий передает ее на второй участок.

2. На втором участке 1 ≤ L ≤ 5 работников осуществляют заточку. Булавка попадает случайно выбранному из них свободному работнику, который осуществляет заточку и передает заточенную булавку на третий участок.

3. На третьем участке 1 ≤ M ≤ 2 работников осуществляют контроль качества операции.

Требуется создать многопроцессное приложение, моделирующее работу цеха. Каждый работник — это отдельный процесс. При решении использовать парадигму "производитель-потребитель". Следует учесть, что каждая из операций выполняется за случайное время, которое не связано с конкретным рабочим. Передача булавок осуществляется непосредственно от одного рабочего другому. То есть, если все рабочие на определенном участке заняты, то осуществляется ожидание одного из тех, кто освободится.

## Часть на оценку 4-5
Код реализует многопроцессное приложение, моделирующее работу цеха по производству булавок. Используется парадигма "производитель-потребитель". Программа создает несколько процессов для каждого этапа производства: проверка, заточка и контроль качества. Каждый процесс работает с общей памятью и использует семафоры для синхронизации доступа к ней.

**В общем:**
   Множество процессов взаимодействуют с использованием неименованных POSIX семафоров расположенных в разделяемой памяти. Обмен данными также ведется через разделяемую память в стандарте POSIX.
   
### Реализация

- Создание семафора: Используется функция semget для создания неименованного семафора. Если создание не удалось, программа выходит с ошибкой.

- Инициализация семафора: Используется функция semctl для установки начального значения семафора. Если инициализация не удалась, программа выходит с ошибкой.

- Создание разделяемой памяти: Используется функция shmget для создания сегмента разделяемой памяти. Если создание не удалось, программа выходит с ошибкой.

- Прикрепление разделяемой памяти: Используется функция shmat для прикрепления сегмента разделяемой памяти к адресному пространству процесса. Если прикрепление не удалось, программа выходит с ошибкой.

- Создание процессов для каждого этапа производства: Для каждого этапа создается несколько процессов, которые работают с общей памятью. Каждый процесс ожидает доступа к памяти с помощью семафора, затем выполняет свою функцию (проверка, заточка, контроль качества) и освобождает семафор.

- Ожидание завершения процессов: Родительский процесс ожидает завершения всех дочерних процессов с помощью функции wait.

- Отсоединение разделяемой памяти: Используется функция shmdt для отсоединения сегмента разделяемой памяти от адресного пространства процесса. Если отсоединение не удалось, программа выходит с ошибкой.

- Удаление разделяемой памяти и семафора: Используются функции shmctl и semctl для удаления разделяемой памяти и семафора соответственно.

## Часть на оценку 6-7

**В общем:**
    Множество процессов взаимодействуют с использованием именованных POSIX семафоров. Обмен данными ведется через разделяемую память в стандарте POSIX. 

   
### Реализация
- Создание именованного семафора: Используется функция sem_open для создания именованного семафора с правами доступа 0666. Если создание не удалось, программа выходит с ошибкой.
  
- Создание разделяемой памяти: Используется функция shmget для создания сегмента разделяемой памяти. Если создание не удалось, программа выходит с ошибкой.
  
- Прикрепление разделяемой памяти: Используется функция shmat для прикрепления сегмента разделяемой памяти к адресному пространству процесса. Если прикрепление не удалось, программа выходит с ошибкой.
  
- Создание процессов для каждого этапа производства: Для каждого этапа создается несколько процессов, которые работают с общей памятью. Каждый процесс ожидает доступа к памяти с помощью семафора, затем выполняет свою функцию (проверка, заточка, контроль качества) и освобождает семафор.

- Ожидание завершения процессов: Родительский процесс ожидает завершения всех дочерних процессов с помощью функции wait.

- Отсоединение разделяемой памяти: Используется функция shmdt для отсоединения сегмента разделяемой памяти от адресного пространства процесса. Если отсоединение не удалось, программа выходит с ошибкой.

- Удаление разделяемой памяти и семафора: Используются функции shmctl и sem_unlink для удаления разделяемой памяти и именованного семафора соответственно.



## Часть на оценку 8

**В общем:**
   Разработано приложение, состоящее из отдельных программ–
процессов, запускаемых независимо друг от друга. Множество независимых процессов взаимодействуют с использованием семафоров в стандарте UNIX SYSTEM V. Обмен данными ведется через разделяемую память в стандарте UNIX
SYSTEM V.


- Каждая программа работает в отдельном процессе и независимо друг от друга.
- Взаимодействие между процессами осуществляется с помощью семафоров и разделяемой памяти в стандарте UNIX System V.
- Программы выполняются параллельно.
- После завершения работы все ресурсы освобождаются для последующего использования.
### Реализация

1. **Программа проверки кривизны булавок (`check_pin.c`):**
   - Создает семафор и разделяемую память.
   - Осуществляет проверку случайного количества булавок на предмет кривизны.
   - Записывает результаты проверки в разделяемую память.
   - Освобождает ресурсы (семафор и разделяемую память) после завершения работы.

2. **Программа заточки булавок (`sharpen_pin.c`):**
   - Получает доступ к семафору и разделяемой памяти, созданным программой проверки.
   - Осуществляет заточку булавок, если они не кривые.
   - Записывает результаты заточки в разделяемую память.
   - Освобождает ресурсы после завершения работы.

3. **Программа контроля качества булавок (`control_pin.c`):**
   - Получает доступ к семафору и разделяемой памяти.
   - Осуществляет контроль качества заточенных булавок.
   - Записывает результаты контроля в разделяемую память.
   - Освобождает ресурсы после завершения работы.


## Часть на оценку 9

**В общем:**
    Множество независимых процессов взаимодействуют с использованием именованных POSIX семафоров. Обмен данными ведется через каналы или очереди сообщений в стандарте POSIX.

### Реализация

1. **Программа проверки кривизны булавок (`check_pin.c`):**
   - Создается именованный POSIX семафор для синхронизации доступа к разделяемой памяти.
   - Создается разделяемая память для хранения информации о булавках.
   - Процесс проверки булавок выполняет следующие шаги:
     - Ожидает доступа к разделяемой памяти, блокируя семафор.
     - Проводит проверку каждой булавки на предмет кривизны.
     - Записывает результаты проверки в разделяемую память.
     - Освобождает семафор.

2. **Программа заточки булавок (`sharpen_pin.c`):**
   - Открывается именованный POSIX семафор, созданный процессом проверки.
   - Получает доступ к разделяемой памяти, созданной процессом проверки.
   - Процесс заточки булавок выполняет следующие шаги:
     - Ожидает доступа к разделяемой памяти, блокируя семафор.
     - Проводит заточку каждой булавки, если она не кривая.
     - Записывает результаты заточки в разделяемую память.
     - Освобождает семафор.

3. **Программа контроля качества булавок (`control_pin.c`):**
   - Открывается именованный POSIX семафор, созданный процессом проверки.
   - Получает доступ к разделяемой памяти, созданной процессом проверки.
   - Процесс контроля качества булавок выполняет следующие шаги:
     - Ожидает доступа к разделяемой памяти, блокируя семафор.
     - Проводит контроль качества каждой заточенной булавки.
     - Записывает результаты контроля в разделяемую память.
     - Освобождает семафор.


## Часть на оценку 10

5. **В общем:**
   Множество независимых процессов взаимодействуют с использованием семафоров в стандарте UNIX SYSTEM V. Обмен данными ведется через через каналы или очереди сообщений в стандарте UNIX SYSTEM V.

### Реализация
Программа моделирует производственный процесс булавок, включающий три этапа: проверка кривизны, заточка и контроль качества. Каждый этап выполняется отдельным процессом, который запускается независимо от других и взаимодействует с ними через семафоры и очереди сообщений в стандарте UNIX System V.

1. **Проверка кривизны булавок (`check_pin.c`):**
   - Создается семафор и очередь сообщений.
   - Процесс проверки кривизны булавок получает доступ к разделяемой очереди сообщений и ожидает поступления заданий на проверку.
   - При получении задания, процесс проверяет кривизну булавки и отправляет результаты проверки обратно через очередь сообщений.

2. **Заточка булавок (`sharpen_pin.c`):**
   - Создается семафор и очередь сообщений.
   - Процесс заточки булавок получает доступ к разделяемой очереди сообщений и ожидает поступления заданий на заточку.
   - При получении задания, процесс заточивает булавку (если она не кривая) и отправляет результаты заточки обратно через очередь сообщений.

3. **Контроль качества булавок (`control_pin.c`):**
   - Создается семафор и очередь сообщений.
   - Процесс контроля качества булавок получает доступ к разделяемой очереди сообщений и ожидает поступления заданий на контроль качества.
   - При получении задания, процесс проверяет качество заточенной булавки и отправляет результаты контроля обратно через очередь сообщений.

   
