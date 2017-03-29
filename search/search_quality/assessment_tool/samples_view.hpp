#pragma once

#include "search/search_quality/assessment_tool/context.hpp"

#include "base/checked_cast.hpp"

#include <cstddef>
#include <vector>

#include <QtGui/QStandardItemModel>
#include <QtWidgets/QTableView>

class SamplesView : public QTableView
{
public:
  explicit SamplesView(QWidget * parent);

  void SetSamples(ContextList::SamplesSlice const & samples) { m_model->SetSamples(samples); }
  bool IsSelected(size_t index) const;
  void OnUpdate(size_t index) { m_model->OnUpdate(index); }

private:
  class Model : public QStandardItemModel
  {
  public:
    explicit Model(QWidget * parent);

    void SetSamples(ContextList::SamplesSlice const & samples)
    {
      removeRows(0, rowCount());
      m_samples = samples;
      if (m_samples.IsValid())
        insertRows(0, base::checked_cast<int>(m_samples.Size()));
    }

    void OnUpdate(size_t index)
    {
      auto const ix = createIndex(base::checked_cast<int>(index) /* row */, 0 /* column */);

      // Need to refresh view when some item is updated.
      emit dataChanged(ix, ix);
    }

    // QStandardItemModel overrides:
    QVariant data(QModelIndex const & index, int role = Qt::DisplayRole) const override;

  private:
    ContextList::SamplesSlice m_samples;
  };

  Model * m_model = nullptr;
};
