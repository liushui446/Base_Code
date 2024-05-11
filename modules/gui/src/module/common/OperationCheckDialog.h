#ifndef AS_OPERATIONCHECKDIALOG_H
#define AS_OPERATIONCHECKDIALOG_H

#include "CommonPart.h"
#include "as/common/UiCallback.h"
//#include "as/RangeCore.hpp"

namespace Ui {
	class OperationCheckDialog;
}
class OperationCheckDialog : public ASQDialog
{
    Q_OBJECT

public:
    enum class KeyValue
    {
        Yes,
        YesToAll,
        No,
        Cancel,
        None
    };

    explicit OperationCheckDialog(as::WidgetType widget_type, QString info, int key_amount, QWidget* parent, bool isSplit = true);
    ~OperationCheckDialog();

    KeyValue GetKeyValue();

private:
    Ui::OperationCheckDialog *m_ui;
    QImage* img;
    KeyValue m_keyValue;
};

class JudgeRangePart
{
public:
    //JudgeRangePart(as::RangeTab rangedb);
    JudgeRangePart(string rangemin, string rangemax);
    JudgeRangePart(string rangemin, string rangemax, string rangedefault);
    int JudgeRangevalue(string dialogname, string LineeditName, string input);
    string Rangedefault = "0.000";
private:
    string Rangemin;
    string Rangemax;
};

#endif // OPERATIONCHECKDIALOG_H
