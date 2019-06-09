#ifndef PROCCESSLIST_H
#define PROCCESSLIST_H

#include <QtWidgets/QtWidgets>
#include <vector>

struct ProccessTemplate
{
    unsigned ID;
    unsigned arrivalTime;
    unsigned duration;
    unsigned deadline;
    unsigned numberOfPages;
};

class ProccessList : public QScrollArea
{
    Q_OBJECT

public:
    ProccessList(QWidget* parent = nullptr);
    void AddProccess(int index);
	void AddProccessList(
		const std::vector<ProccessTemplate>& proccessList
	);
    std::vector<ProccessTemplate> GetCurrentProccesses();
    void ClearProccesses();

signals:
    void NumberOfProccessesChanged(int value);

private slots:
    void RemoveProccess(QWidget* widget);

private:
    QHBoxLayout* m_mainLayout;;
    QWidget* m_centralWidget;
    
    int m_numberOfProccesses;

    void CreateMainLayout();
};

#endif
