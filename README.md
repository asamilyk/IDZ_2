# Операционная система ИДЗ-2 "Самилык Анастасия" БПИ229

## Вариант 28
## Работа выполнена на оценку 10.

### Индивидуальное условие: Задача о производстве булавок

В цехе по заточке булавок все необходимые операции осуществляются рабочими на трех производственных участках.

1. На первом участке 1 ≤ K ≤ 3 рабочих получают тупые булавки и в течение некоторого случайного времени каждый из них проверяет ее на предмет кривизны. Если булавка не кривая, то рабочий передает ее на второй участок.

2. На втором участке 1 ≤ L ≤ 5 работников осуществляют заточку. Булавка попадает случайно выбранному из них свободному работнику, который осуществляет заточку и передает заточенную булавку на третий участок.

3. На третьем участке 1 ≤ M ≤ 2 работников осуществляют контроль качества операции.

Требуется создать многопроцессное приложение, моделирующее работу цеха. Каждый работник — это отдельный процесс. При решении использовать парадигму "производитель-потребитель". Следует учесть, что каждая из операций выполняется за случайное время, которое не связано с конкретным рабочим. Передача булавок осуществляется непосредственно от одного рабочего другому. То есть, если все рабочие на определенном участке заняты, то осуществляется ожидание одного из тех, кто освободится.

## Часть на оценку 4-5
**Отчет по коду: Многопроцессное приложение для моделирования работы цеха по заточке булавок**

1. **Введение:**
   
   Данный код реализует многопроцессное приложение на языке C для моделирования работы цеха по заточке булавок. В цехе предполагается три этапа: проверка кривизны булавок, заточка и контроль качества. Каждый этап представлен отдельным процессом. Взаимодействие между процессами осуществляется с помощью семафоров и разделяемой памяти.

2. **Создание семафора и разделяемой памяти:**

   - Функция `create_semaphore()` создает семафор с помощью системного вызова `semget()`. Семафор используется для синхронизации доступа к разделяемой памяти.
   
   - Функция `initialize_shared_memory()` инициализирует разделяемую память с помощью системных вызовов `shmget()` и `shmat()`. Разделяемая память используется для хранения информации о состоянии булавок (обработаны или нет).

3. **Проверка кривизны булавок:**

   - Функция `check_curvature()` моделирует работу рабочего, который проверяет кривизну булавок. Рабочий получает доступ к разделяемой памяти с помощью семафора, чтобы проверить булавки, которые не были обработаны другими рабочими.
   
   - В этой функции используется механизм POSIX-потоков для создания нескольких рабочих, каждый из которых представлен отдельным потоком.

5. **В общем:**
   Множество процессов взаимодействуют с использованием неименованных POSIX семафоров расположенных в разделяемой памяти. Обмен данными также ведется через разделяемую память в стандарте POSIX.

