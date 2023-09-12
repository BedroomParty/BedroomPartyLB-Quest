#include "PluginUI/SeasonList/SeasonListData.hpp"
#include "PluginUI/SeasonList/SeasonListCell.hpp"
#include "assets.hpp"

#include "System/Action_2.hpp"
#include "bsml/shared/Helpers/utilities.hpp"

#include "custom-types/shared/delegate.hpp"

#include "Utils/Constants.hpp"
#include "Utils/StringUtils.hpp"

DEFINE_TYPE(BedroomPartyLB::UI, SeasonListData);

namespace BedroomPartyLB::UI
{
    void SeasonListData::ctor()
    {
        INVOKE_CTOR();
        reuseIdentifier = StringW("SeasonListCell");
        cellSize = 9.0f;
    }

    float SeasonListData::CellSize()
    {
        return cellSize;
    }

    int SeasonListData::NumberOfCells()
    {
        return seasonList.size();
    }

    HMUI::TableCell* SeasonListData::CellForIdx(HMUI::TableView* tableView, int idx)
    {
        auto tableCell = reinterpret_cast<SeasonListCell*>(tableView->DequeueReusableCellForIdentifier(reuseIdentifier));

        if (!tableCell)
        {
            tableCell = SeasonListCell::CreateNewCell();
        }

        tableCell->tableData = this;
        tableCell->Populate(seasonList[idx]);
        tableCell->set_reuseIdentifier(reuseIdentifier);

        return tableCell;
    }

    HMUI::TableView::IDataSource* SeasonListData::i_IDataSource() { return reinterpret_cast<HMUI::TableView::IDataSource*>(this); }

}