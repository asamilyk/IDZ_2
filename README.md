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
   - Для синхронизации доступа к разделяемой памяти между процессами создается семафор с помощью системного вызова semget.
   - Для хранения информации о булавках используется разделяемая память, которая создается с помощью системного вызова shmget.
   - Булавка представлена структурой Pin, которая содержит информацию о кривизне, заточке и проверке качества.
   - Для каждого этапа производства (проверка, заточка, контроль качества) создается определенное количество рабочих процессов.
   - Рабочие процессы выполняют операции над булавками в разделяемой памяти.
   - Семафор используется для блокировки доступа к разделяемой памяти во время выполнения операций над булавками.

**В общем:**
   Множество процессов взаимодействуют с использованием именованных POSIX семафоров расположенных в разделяемой памяти. Обмен данными также ведется через разделяемую память в стандарте POSIX.


## Часть на оценку 6-7

- Создание именованного семафора: Используется функция sem_open для создания именованного семафора с правами доступа 0666. Если создание не удалось, программа выходит с ошибкой.
  
- Создание разделяемой памяти: Используется функция shmget для создания сегмента разделяемой памяти. Если создание не удалось, программа выходит с ошибкой.
  
- Прикрепление разделяемой памяти: Используется функция shmat для прикрепления сегмента разделяемой памяти к адресному пространству процесса. Если прикрепление не удалось, программа выходит с ошибкой.
  
- Создание процессов для каждого этапа производства: Для каждого этапа создается несколько процессов, которые работают с общей памятью. Каждый процесс ожидает доступа к памяти с помощью семафора, затем выполняет свою функцию (проверка, заточка, контроль качества) и освобождает семафор.

- Ожидание завершения процессов: Родительский процесс ожидает завершения всех дочерних процессов с помощью функции wait.

- Отсоединение разделяемой памяти: Используется функция shmdt для отсоединения сегмента разделяемой памяти от адресного пространства процесса. Если отсоединение не удалось, программа выходит с ошибкой.

- Удаление разделяемой памяти и семафора: Используются функции shmctl и sem_unlink для удаления разделяемой памяти и именованного семафора соответственно.

**В общем:**
    Множество процессов взаимодействуют с использованием именованных POSIX семафоров. Обмен данными ведется через разделяемую память в стандарте POSIX. 

## Часть на оценку 8

5. **В общем:**
   Множество процессов взаимодействуют с использованием неименованных POSIX семафоров расположенных в разделяемой памяти. Обмен данными также ведется через разделяемую память в стандарте POSIX.


## Часть на оценку 9

5. **В общем:**
   Множество процессов взаимодействуют с использованием неименованных POSIX семафоров расположенных в разделяемой памяти. Обмен данными также ведется через разделяемую память в стандарте POSIX.


## Часть на оценку 10

5. **В общем:**
   Множество процессов взаимодействуют с использованием неименованных POSIX семафоров расположенных в разделяемой памяти. Обмен данными также ведется через разделяемую память в стандарте POSIX.
