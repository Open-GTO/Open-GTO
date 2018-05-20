# Open-GTO

[![GitHub version](https://badge.fury.io/gh/Open-GTO%2FOpen-GTO.svg)](http://badge.fury.io/gh/Open-GTO%2FOpen-GTO)
[![Build Status](https://travis-ci.org/Open-GTO/Open-GTO.svg?branch=master)](https://travis-ci.org/Open-GTO/Open-GTO)
[![Join the chat at https://gitter.im/Open-GTO/Open-GTO](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/Open-GTO/Open-GTO?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Open-GTO – свободный, некоммерческий проект, направленный на создание по-настоящему весёлого игрового режима в экосистеме SA-MP. Проект является продолжением разработки популярного в своё время игрового режима GTO (Grand Theft Online), версии v0.6.0.

# "Ночные" сборки

Периодически собираются сборки игрового режима в полной комплектации (сервер, плагины). Скачать их можно [здесь](https://mega.nz/#F!5IB0RDIB!CiFTx7NlF8mgyXckI76aJw) (чтобы определить последнюю сборку сортируйте их по дате).

# Самостоятельная сборка и запуск

Загрузка всех исходных кодов:
```
git clone https://github.com/Open-GTO/Open-GTO.git
```
(или вы можете использовать [GitHub Desktop](https://desktop.github.com/))

Для установки всех зависимостей и сборки необходимо установить [sampctl](https://github.com/Southclaws/sampctl) и выполнить:
```
sampctl package ensure
sampctl package build
sampctl server ensure
```

Для запуска сервера необходимо ввести команду `sampctl s run`, предварительно настроив `samp.json`.
