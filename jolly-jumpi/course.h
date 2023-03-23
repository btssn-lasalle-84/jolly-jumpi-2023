#ifndef COURSE_H
#define COURSE_H

#include <QObject>

class Course : public QObject
{
    Q_OBJECT

  public:
    Course(QObject* parent = 0);
    ~Course();

  public slots:
};

#endif // COURSE_H
