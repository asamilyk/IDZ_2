# Операционная система ИДЗ-2 "Самлик Анастасия" БПИ229

## Вариант 28
## Работа выполнена на оценку 10.

### Индивидуальное условие: Задача о производстве булавок

В цехе по заточке булавок все необходимые операции осуществляются рабочими на трех производственных участках.

1. На первом участке 1 ≤ K ≤ 3 рабочих получают тупые булавки и в течение некоторого случайного времени каждый из них проверяет ее на предмет кривизны. Если булавка не кривая, то рабочий передает ее на второй участок.

2. На втором участке 1 ≤ L ≤ 5 работников осуществляют заточку. Булавка попадает случайно выбранному из них свободному работнику, который осуществляет заточку и передает заточенную булавку на третий участок.

3. На третьем участке 1 ≤ M ≤ 2 работников осуществляют контроль качества операции.

Требуется создать многопроцессное приложение, моделирующее работу цеха. Каждый работник — это отдельный процесс. При решении использовать парадигму "производитель-потребитель". Следует учесть, что каждая из операций выполняется за случайное время, которое не связано с конкретным рабочим. Передача булавок осуществляется непосредственно от одного рабочего другому. То есть, если все рабочие на определенном участке заняты, то осуществляется ожидание одного из тех, кто освободится.
