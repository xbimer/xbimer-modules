#ifndef MULTIITEMTREEVIEWSELECTIONTABPAGE_HPP
#define MULTIITEMTREEVIEWSELECTIONTABPAGE_HPP

#include "DGModule.hpp"
#include "UDDefinitions.hpp"
#include "ItemSelectionDialog.hpp"
#include "DGRadioItem.hpp"
#include "UDModule.hpp"
#include "TreeViewMultiItemSelectionDialog.hpp"

namespace UD {


class UD_DLL_EXPORT TreeViewMultiItemSelectionTabPageData : public BaseItemSelectionData
{
	friend class TreeViewMultiItemSelectionTabPage;
public:
	enum ColumnIndex
	{
		GroupColumnIndex = 1,
		ItemColumnIndex = 2,
		ListBoxColumnCount = 2
	};

	enum class NodeType
	{
		SelectableSeparately,
		NotSelectableSeparately
	};

	class UD_DLL_EXPORT ItemStatus
	{
	public:
		UD::ItemCheckStatus	GetOriginalStatus () const;
		void				SetOriginalStatus (UD::ItemCheckStatus originalStatus);
		UD::ItemCheckStatus	GetCurrentStatus () const;
		void				SetCurrentStatus (UD::ItemCheckStatus currentStatus);
		bool				IsExpanded () const;
		void				SetIsExpanded (bool isExpanded);

		UD::TreeViewMultiItemSelectionTabPageData::NodeType	GetNodeType () const;
		void												SetNodeType (UD::TreeViewMultiItemSelectionTabPageData::NodeType nodeType);

		ItemStatus (ItemCheckStatus currentStatus, NodeType nodeType, bool isExpanded);
	private:
		ItemCheckStatus			originalStatus;
		ItemCheckStatus			currentStatus;
		bool					isExpanded;
		NodeType			nodeType;
	};

	TreeViewMultiItemSelectionTabPageData ();
	virtual ~TreeViewMultiItemSelectionTabPageData ();

	UIndex AddItem (const AvailableItemData& itemData, NodeType nodeType, ItemCheckStatus currentStatus);
	UIndex AddGroup (const AvailableItemData& itemData, NodeType nodeType, ItemCheckStatus currentStatus);

	void					SetFilterMode (ItemSelectionFilterMode inMode);
	ItemSelectionFilterMode GetFilterMode () const;

	ItemCheckStatus		ToggleItemStatus (UIndex itemIndex);
	void				SetItemExpansionStatus (UIndex itemIndex, bool status);
	void				SetStatusToItemChildren (UIndex itemIndex, UD::ItemCheckStatus status);
	ItemCheckStatus		GetItemStatus (UIndex itemIndex) const;
	void				SetItemStatus (UIndex itemIndex, UD::ItemCheckStatus status);
	ItemCheckStatus		GetItemOriginalStatus (UIndex itemIndex) const;
	void				SetItemOriginalStatus (UIndex itemIndex, UD::ItemCheckStatus status);
	ItemCheckStatus		GetAggregatedChildrenStatus (UIndex itemIndex) const;

	static short		GetIconColumnMinWidth ();
	bool				IsItemSelectable (UIndex itemIndex) const;
	bool				IsItemExpanded (UIndex itemIndex) const;
	bool				AllChildrenHaveEqualStatus (UIndex itemIndex) const;

	const GS::Array<ItemStatus>& GetItemStatuses () const;
	void				  		 SetItemStatuses (const GS::Array<ItemStatus> itemStatuses);
	void				  		 ActualizeOriginalStatusesToCurrent ();
	GS::Array<UIndex>	GetChangedItemIndices () const;

	DG::Icon			GetStatusIconForListDisplay (UIndex itemIndex) const;

private:
	ItemSelectionFilterMode filterMode;
	GS::Array<ItemStatus> itemStatuses;
};


class UD_DLL_EXPORT	TabPageClickStateMachine
{
	UIndex currentFunctionIndex = 0;
	GS::Array<std::function<void ()>> stepFunctions;
	TreeViewMultiItemSelectionTabPageData& TabPageData;
	GS::Array<TreeViewMultiItemSelectionTabPageData::ItemStatus> originalState;
	UIndex itemIndex;

public:
	TabPageClickStateMachine (TreeViewMultiItemSelectionTabPageData& TabPageData, UIndex itemIndex);

	virtual void DoNextStep ();

	void AddToggleStep ();
	void AddToggleChildrenStep ();
	void AddApplyItemStatusToChildrenStep ();
	void AddRestoreStateStep ();
	UIndex GetItemIndex () const;
};




class UD_DLL_EXPORT TreeViewMultiItemSelectionTabPage : public DG::TabPage,
														public SearchableTreeViewAndListBoxDialogBase,
														public DG::ButtonItemObserver,
														public DG::RadioItemObserver
{
public:
	TreeViewMultiItemSelectionTabPage (TreeViewMultiItemSelectionTabPageData& TabPageData, const DG::TabControl& tabControl, GSResModule resModule, GSResModule iconResModule,
									   short tabPageResId, short treeViewButtonResId,
									   short filterToCheckedButtonResId, short listViewButtonResId,
									   short searchInputResId, short treeViewResId,
									   short searchResultResId, short listBoxResId,
									   short separatorId);
	~TreeViewMultiItemSelectionTabPage ();

	virtual void PanelOpened (const DG::PanelOpenEvent& ev) override;
	virtual void PanelResized (const DG::PanelResizeEvent& ev) override;
	virtual void PanelResizeExited (const DG::PanelResizeEvent& ev) override;
	virtual void PanelIdle (const DG::PanelIdleEvent& ev) override;

	virtual void ButtonClicked (const DG::ButtonClickEvent& ev) override;
	virtual void RadioItemChanged (const DG::RadioItemChangeEvent& ev) override;

	virtual void TreeViewItemClicked (const DG::TreeViewItemClickEvent& ev, bool* denySelectionChange) override;
	virtual void TreeViewItemDoubleClicked (const DG::TreeViewDoubleClickEvent& ev, bool* processed) override;

	virtual void ListBoxClicked (const DG::ListBoxClickEvent& ev) override;
	virtual void TreeViewContextMenuRequested (const DG::TreeViewContextMenuEvent& ev, bool* processed) override;

protected:
	DG::IconPushRadio		treeViewButton;
	DG::IconPushRadio		filterToCheckedButton;
	DG::IconPushRadio		listViewButton;

	DG::TextEdit			searchInputTextEdit;
	DG::SingleSelTreeView	itemsTreeView;
	DG::LeftText			searchResultText;
	DG::SingleSelListBox	itemsListBox;
	DG::Separator			separator;

	bool refreshOnIdle;
	TreeViewMultiItemSelectionTabPageData& TabPageData;

	GS::Ref<TabPageClickStateMachine> clickStateMachine;

	void UpdateStatusIconsInTreeView (UIndex itemIndex);
	void UpdateAllStatusIcons ();

	virtual	SearchableTreeViewAndListBoxDialogBase::ItemsDisplayMode GetItemsDisplayMode (void) const override;

	virtual	void	TextEditChanged (const DG::TextEditChangeEvent& ev) override;
	DG::Icon		GetStatusIconForTreeDisplay (UIndex itemIndex) const;

	void	ApplyTreeItemClick (Int32 treeItemId);
	void	SetItemExpansionStatus (UIndex dataIndex, bool status);
	void	SelectTreeItemsRecursively (UIndex itemInd);
	void	DeselectTreeItemsRecursively (UIndex itemInd);

	void ResetClickStateMachine ();

protected:
	virtual void TreeViewItemCollapsed (const DG::TreeViewExpandEvent& ev) override;


	virtual void TreeViewItemExpanded (const DG::TreeViewExpandEvent& ev) override;
};


} // namespace UD


#endif // ITEMSTATUSCHANGERTabPage_HPP
