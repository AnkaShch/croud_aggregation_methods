# croud_aggregation_methods
Methods of aggregating responses to the users of Yandex.Toloka

Реализация трех разных метолов агрегации ответов:
- голос большинства (Majority Vote, MV)
- модель Wawa
- модель Дэвида-Скина

В качестве примера используются два открытых набора данных: : Toloka Aggregation Relevance 2 (TlkAgg2) и Toloka Aggregation Relevance 5 (TlkAgg5).

Структура проекта:
- `TlkAgg2/{crowd_labels, golden_labels}.tsv` — полученные от пользователей ответы, эталонные ответы по набору данных TlkAgg2
- `TlkAgg5/{crowd_labels, golden_labels}.tsv` — полученные от пользователей ответы, эталонные ответы по набору данных TlkAgg5
- `TlkAgg2/{mv,wawa,ds}.tsv` — результаты по набору данных TlkAgg2
- `TlkAgg5/{mv,wawa,ds}.tsv` — результаты по набору данных TlkAgg5
- `MV/` — реализация метода MV
- `Wawa/` — реализация метода Wawa
- `DS/` — реализация метода DS
