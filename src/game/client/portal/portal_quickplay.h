//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Quickplay client UI
//
// $NoKeywords: $
//=============================================================================//

#ifndef _INCLUDED_PORTAL_QUICKPLAY_UI_H
#define _INCLUDED_PORTAL_QUICKPLAY_UI_H
#ifdef _WIN32
#pragma once
#endif

namespace vgui
{
	class Button;
	class ComboBox;
	class RadioButton;
}

#include "vgui_controls/EditablePanel.h"
#include "vgui_controls/ComboBox.h"
#include "portal_quickplay_shared.h"

// Matchmaking stats
struct TF_Gamestats_QuickPlay_t
{
	// Status code for the search as a whole
	enum eResult
	{
		k_Result_InternalError = -1,
		k_Result_UserCancel = 10,
		k_Result_NoServersFound = 20,
		k_Result_NoServersMetCrtieria = 30,
		//k_Result_NeverHeardBackFromGC = 40,
		//k_Result_ReceivedZeroGCScores = 50,
		k_Result_FinalPingFailed = 60,
		k_Result_TriedToConnect = 100,
	};

	// Status codes for the servers
	enum eServerStatus
	{
		k_Server_Invalid = -1, // we have a bug if this gets reported
		k_Server_Ineligible = 10,
		k_Server_Eligible = 20,
		k_Server_RequestedScore = 30,
		k_Server_Scored = 40,
		k_Server_Pinged = 50,
		k_Server_PingTimedOut = 60,
		k_Server_PingIneligible = 70,
		k_Server_Connected = 100,
	};

	TF_Gamestats_QuickPlay_t()
	{
		m_sUserGameMode;
		m_fSearchTime = -1.0;
		m_eResultCode = k_Result_UserCancel;
		m_iExperimentGroup = 0;
	}

	CUtlString m_sUserGameMode;
	float m_fSearchTime;
	eResult m_eResultCode;
	int m_iExperimentGroup; // TF2ScoringNumbers_t::ExperimentGroup_t

	struct Server_t
	{
		uint32 m_ip;
		uint16 m_port;
		bool m_bRegistered;
		bool m_bValve;
		bool m_bSecure;
		bool m_bMapIsQuickPlayOK;
		int m_nPlayers;
		int m_nMaxPlayers;
		CUtlString m_sMapName;
		CUtlString m_sTags;
		int m_iPing;
		float m_fScoreClient;
		float m_fScoreServer;
		float m_fScoreGC;
		eServerStatus m_eStatus;
	};
	CUtlVector<Server_t> m_vecServers;
};

struct QuickplayItem
{
	const char *pTitle;
	const char *pDescription;
	const char *pMoreInfo;
	const char *pComplexity;
	const char *pImage;
	const char *pBetaImage;
	EGameCategory gameType;
};

//-----------------------------------------------------------------------------
// Purpose: Quickplay Dialog
//-----------------------------------------------------------------------------
extern ConVar tf_quickplay_lastviewedmode;

class CQuickplayPanelBase : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE( CQuickplayPanelBase, vgui::EditablePanel );
public:
	CQuickplayPanelBase( vgui::Panel *parent, const char *name );
	virtual ~CQuickplayPanelBase();

	virtual void ApplySettings( KeyValues *pInResourceData ) OVERRIDE;
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme ) OVERRIDE;
	virtual void OnCommand( const char *pCommand );

	void ShowItemByGameType( EGameCategory gameType );
	void ShowItemByIndex( int iItem );
	int GetSelectedItemIndex() const { return m_iCurrentItem; }

	void SetPageScrollButtonsVisible( bool bFlag );

protected:
	void SetupActionTarget( const char *pPanelName );
	void AddItem( EGameCategory gameType, const char *pTitle, const char *pDescription, const char *pMoreInfo, const char *pComplexity, const char *pImage, const char *pBetaImage );
	virtual void UpdateSelectableItems();
	void SaveSettings();
	void SetupMoreOptions();
	void ReadOptionCombos();
	void WriteOptionCombosAndSummary();
	virtual void GetOptionsAndSummaryText( wchar_t *pwszSummary );
	void ShowSimplifiedOrAdvancedOptions();
	virtual const char *GetItemImage( const QuickplayItem& item ) const;
	MESSAGE_FUNC_PTR( OnTextChanged, "TextChanged", panel );
	MESSAGE_FUNC_PTR( OnRadioButtonChecked, "RadioButtonChecked", panel );

	virtual void UserSelectItemByIndex( int iNewItem );

protected:

	vgui::EditablePanel *m_pContainer;
	vgui::EditablePanel *m_pSimplifiedOptionsContainer;
	vgui::EditablePanel *m_pAdvOptionsContainer;
	vgui::EditablePanel *m_pGameModeInfoContainer;
	vgui::Button *m_pPrevPageButton;
	vgui::Button *m_pNextPageButton;
	vgui::Button *m_pMoreOptionsButton;
	vgui::EditablePanel *m_pMoreInfoContainer;
	vgui::ComboBox *m_pGameModeCombo;
	vgui::Label *m_pOptionsSummaryLabel;

	enum EAdvOption
	{
		kEAdvOption_ServerHost,
		kEAdvOption_MaxPlayers,
		kEAdvOption_Respawn,
		kEAdvOption_RandomCrits,
		kEAdvOption_DamageSpread
	};

	struct AdvOption {
		const char *m_pszContainerName;
		ConVar *m_pConvar;
		CUtlVector<const char *> m_vecOptionNames;
		CUtlVector<vgui::RadioButton *> m_vecRadioButtons;
		CUtlVector<const char *> m_vecOptionSummaryNames;
		int m_nChoice;
	};
	CUtlVector<AdvOption> m_vecAdvOptions;

	//vgui::CheckButton *m_pFavoritesCheckButton;
	//vgui::Button *m_pRefreshButton;
	CUtlVector< QuickplayItem > m_vecItems;
	CUtlVector< QuickplayItem > m_vecAllItems;
	int m_iCurrentItem;
	bool m_bSetInitialSelection;
	bool m_bShowRandomOption;

	char	m_szEvent247Image[MAX_PATH];
	char	m_szCommunityUpdateImage[MAX_PATH];
};

#endif // #ifndef _INCLUDED_PORTAL_QUICKPLAY_UI_H
