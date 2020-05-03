#pragma once
namespace Soul
{
	//Event From gui mouse keyboard
	enum E_EVENT_TYPE
	{
		GUI_EVENT = 0,
		MOUSE_INPUT_EVENT,
		KEY_INPUT_EVENT
	};
	//mouse event
	enum E_MOUSE_INPUT_EVENT
	{
		//Left mouse button was pressed down.
		E_MOUSE_LMOUSE_PRESSED_DOWN = 0,

		//Right mouse button was pressed down.
		E_MOUSE_RMOUSE_PRESSED_DOWN,

		//Middle mouse button was pressed down.
		E_MOUSE_MMOUSE_PRESSED_DOWN,

		//Left mouse button was left up.
		E_MOUSE_LMOUSE_LEFT_UP,

		//Right mouse button was left up.
		E_MOUSE_RMOUSE_LEFT_UP,

		//Middle mouse button was left up.
		E_MOUSE_MMOUSE_LEFT_UP,

		//The mouse cursor changed its position.
		E_MOUSE_MOUSE_MOVED,

		//The mouse wheel was moved. Use Wheel value in event data to find out
		//in what direction and how fast.
		E_MOUSE_MOUSE_WHEEL,

		//No real event. Just for convenience to get number of events
		COUNT,
	};
	namespace Gui
	{
		class IGUIElement;
		//! Enumeration for all events which are sendable by the gui system
		enum EGUI_EVENT_TYPE
		{
			//! A gui element has lost its focus.
			/** GUIEvent.Caller is losing the focus to GUIEvent.Element.
			If the event is absorbed then the focus will not be changed. */
			EGET_ELEMENT_FOCUS_LOST = 0,

			//! A gui element has got the focus.
			/** If the event is absorbed then the focus will not be changed. */
			EGET_ELEMENT_FOCUSED,

			//! The mouse cursor hovered over a gui element.
			/** If an element has sub-elements you also get this message for the subelements */
			EGET_ELEMENT_HOVERED,

			//! The mouse cursor left the hovered element.
			/** If an element has sub-elements you also get this message for the subelements */
			EGET_ELEMENT_LEFT,

			//! An element would like to close.
			/** Windows and context menus use this event when they would like to close,
			this can be cancelled by absorbing the event. */
			EGET_ELEMENT_CLOSED,

			//! A button was clicked.
			EGET_BUTTON_CLICKED,

			//! A scrollbar has changed its position.
			EGET_SCROLL_BAR_CHANGED,

			//! A checkbox has changed its check state.
			EGET_CHECKBOX_CHANGED,

			//! A new item in a listbox was selected.
			/** NOTE: You also get this event currently when the same item was clicked again after more than 500 ms. */
			EGET_LISTBOX_CHANGED,

			//! An item in the listbox was selected, which was already selected.
			/** NOTE: You get the event currently only if the item was clicked again within 500 ms or selected by "enter" or "space". */
			EGET_LISTBOX_SELECTED_AGAIN,

			//! A file has been selected in the file dialog
			EGET_FILE_SELECTED,

			//! A directory has been selected in the file dialog
			EGET_DIRECTORY_SELECTED,

			//! A file open dialog has been closed without choosing a file
			EGET_FILE_CHOOSE_DIALOG_CANCELLED,

			//! 'Yes' was clicked on a messagebox
			EGET_MESSAGEBOX_YES,

			//! 'No' was clicked on a messagebox
			EGET_MESSAGEBOX_NO,

			//! 'OK' was clicked on a messagebox
			EGET_MESSAGEBOX_OK,

			//! 'Cancel' was clicked on a messagebox
			EGET_MESSAGEBOX_CANCEL,

			//! In an editbox 'ENTER' was pressed
			EGET_EDITBOX_ENTER,

			//! The text in an editbox was changed. This does not include automatic changes in text-breaking.
			EGET_EDITBOX_CHANGED,

			//! The marked area in an editbox was changed.
			EGET_EDITBOX_MARKING_CHANGED,

			//! The tab was changed in an tab control
			EGET_TAB_CHANGED,

			//! A menu item was selected in a (context) menu
			EGET_MENU_ITEM_SELECTED,

			//! The selection in a combo box has been changed
			EGET_COMBO_BOX_CHANGED,

			//! The value of a spin box has changed
			EGET_SPINBOX_CHANGED,

			//! A table has changed
			EGET_TABLE_CHANGED,
			EGET_TABLE_HEADER_CHANGED,
			EGET_TABLE_SELECTED_AGAIN,

			//! A tree view node lost selection. See IGUITreeView::getLastEventNode().
			EGET_TREEVIEW_NODE_DESELECT,

			//! A tree view node was selected. See IGUITreeView::getLastEventNode().
			EGET_TREEVIEW_NODE_SELECT,

			//! A tree view node was expanded. See IGUITreeView::getLastEventNode().
			EGET_TREEVIEW_NODE_EXPAND,

			//! A tree view node was collapsed. See IGUITreeView::getLastEventNode().
			EGET_TREEVIEW_NODE_COLLAPSE,

			//! deprecated - use EGET_TREEVIEW_NODE_COLLAPSE instead. This
			//! may be removed by Irrlicht 1.9
			EGET_TREEVIEW_NODE_COLLAPS = EGET_TREEVIEW_NODE_COLLAPSE,

			//! No real event. Just for convenience to get number of events
			EGET_COUNT
		};
	}

	//! SEvents hold information about an event. See irr::IEventReceiver for details on event handling.
	struct SEvent
	{
		//! Any kind of GUI event.
		struct SGUIEvent
		{
			//! IGUIElement who called the event
			Gui::IGUIElement* Caller;

			//! If the event has something to do with another element, it will be held here.
			Gui::IGUIElement* Element;

			//! Type of GUI Event
			Gui::EGUI_EVENT_TYPE EventType;

		};

		//! Any kind of mouse event.
		struct SMouseInput
		{
			//! X position of mouse cursor
			float X;

			//! Y position of mouse cursor
			float Y;

			//! mouse wheel delta, often 1.0 or -1.0, but can have other values < 0.f or > 0.f;
			/** Only valid if event was EMIE_MOUSE_WHEEL */
			float Wheel;

			//! True if shift was also pressed
			bool Shift : 1;

			//! True if ctrl was also pressed
			bool Control : 1;

			//! Type of mouse event
			E_MOUSE_INPUT_EVENT Event;
		};

		//! Any kind of keyboard event.
		struct SKeyInput
		{
			//! True if shift was also pressed
			bool Shift : 1;

			//! True if ctrl was also pressed
			bool Control : 1;
		};

		E_EVENT_TYPE EventType;
		union
		{
			struct SGUIEvent GUIEvent;
			struct SMouseInput MouseInput;
			struct SKeyInput KeyInput;
		};
	};
	class IEventReceiver
	{
	public:
		IEventReceiver() = default;
		virtual ~IEventReceiver() {};
		virtual bool OnEvent(const SEvent& event) = 0;
	};
}