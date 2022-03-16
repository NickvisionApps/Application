#include "xmlstrings.h"

using namespace NickvisionApplication::UI;

std::string XmlStrings::getProgressDialog()
{
    return R"(
        <?xml version="1.0" encoding="UTF-8"?>
        <interface>
            <object class="AdwWindow" id="gtk_progDialog">
                <property name="default-width">400</property>

                <property name="default-height">60</property>

                <property name="modal">true</property>

                <property name="resizable">false</property>

                <property name="deletable">false</property>

                <property name="destroy-with-parent">false</property>

                <child>
                    <object class="GtkBox" id="gtk_boxDiag">
                        <property name="orientation">vertical</property>

                        <child>
                            <object class="GtkLabel" id="gtk_lblDescription">
                                <property name="halign">start</property>

                                <property name="margin-start">10</property>

                                <property name="margin-top">6</property>

                                <property name="margin-end">10</property>

                                <property name="margin-bottom">6</property>
                            </object>
                        </child>

                        <child>
                            <object class="GtkProgressBar" id="gtk_progBar">
                                <property name="margin-start">10</property>

                                <property name="margin-top">10</property>

                                <property name="margin-end">10</property>

                                <property name="margin-bottom">10</property>
                            </object>
                        </child>
                    </object>
                </child>
            </object>
        </interface>
    )";
}

std::string XmlStrings::getMainWindow()
{
    return R"(
        <?xml version="1.0" encoding="UTF-8"?>
        <interface>
            <object class="AdwApplicationWindow" id="adw_winMain">
                <property name="default-width">800</property>

                <property name="default-height">600</property>

                <child>
                    <object class="GtkBox" id="gtk_boxWin">
                        <property name="orientation">vertical</property>

                        <child>
                            <object class="AdwHeaderBar" id="adw_headerBar">
                                <property name="show-end-title-buttons">true</property>

                                <property name="title-widget">
                                    <object class="AdwWindowTitle">
                                        <property name="title" translatable="yes">NickvisionApplication</property>
                                    </object>
                                </property>

                                <child type="end">
                                    <object class="GtkMenuButton" id="gtk_btnHeaderHelp">
                                        <property name="direction">none</property>
                                    </object>
                                </child>
                            </object>
                        </child>

                        <child>
                            <object class="AdwToastOverlay" id="adw_toastOverlay">
                                <property name="vexpand">true</property>

                                <child>
                                    <object class="GtkBox" id="gtk_boxMain">
                                        <property name="orientation">vertical</property>

                                        <property name="spacing">6</property>

                                        <child>
                                            <object class="GtkLabel" id="gtk_lblFirstName">
                                                <property name="halign">start</property>

                                                <property name="margin-start">10</property>

                                                <property name="margin-top">10</property>

                                                <property name="label" translatable="yes">First Name</property>
                                            </object>
                                        </child>

                                        <child>
                                            <object class="GtkEntry" id="gtk_txtFirstName">
                                                <property name="width-request">320</property>

                                                <property name="height-request">20</property>

                                                <property name="margin-start">10</property>

                                                <property name="margin-end">10</property>

                                                <property name="placeholder-text" translatable="yes">Enter first name here</property>
                                            </object>
                                        </child>

                                        <child>
                                            <object class="GtkLabel" id="gtk_lblLastName">
                                                <property name="halign">start</property>

                                                <property name="margin-start">10</property>

                                                <property name="label" translatable="yes">Last Name</property>
                                            </object>
                                        </child>

                                        <child>
                                            <object class="GtkEntry" id="gtk_txtLastName">
                                                <property name="width-request">320</property>

                                                <property name="height-request">20</property>

                                                <property name="margin-start">10</property>

                                                <property name="margin-end">10</property>

                                                <property name="margin-bottom">10</property>

                                                <property name="placeholder-text" translatable="yes">Enter last name here</property>
                                            </object>
                                        </child>
                                    </object>
                                </child>
                            </object>
                        </child>
                    </object>
                </child>
            </object>
        </interface>
    )";
}

std::string XmlStrings::getMenuHelp()
{
    return R"(
        <?xml version="1.0" encoding="UTF-8"?>
        <interface>
            <menu id="gio_menuHelp">
                <section>
                    <item>
                        <attribute name="label" translatable="yes">Check for _Updates</attribute>

                        <attribute name="action">win.checkForUpdates</attribute>
                    </item>
                </section>

                <section>
                    <item>
                        <attribute name="label" translatable="yes">_GitHub Repo</attribute>

                        <attribute name="action">win.gitHubRepo</attribute>
                    </item>

                    <item>
                        <attribute name="label" translatable="yes">_Report a Bug</attribute>

                        <attribute name="action">win.reportABug</attribute>
                    </item>
                </section>

                <section>
                    <item>
                        <attribute name="label" translatable="yes">_Preferences</attribute>

                        <attribute name="action">win.preferences</attribute>
                    </item>

                    <item>
                        <attribute name="label" translatable="yes">_Changelog</attribute>

                        <attribute name="action">win.changelog</attribute>
                    </item>

                    <item>
                        <attribute name="label" translatable="yes">_About</attribute>

                        <attribute name="action">win.about</attribute>
                    </item>
                </section>
            </menu>
        </interface>
    )";
}

std::string XmlStrings::getPreferencesDialog()
{
    return R"(
        <?xml version="1.0" encoding="UTF-8"?>
        <interface>
            <object class="AdwWindow" id="gtk_preferencesDialog">
                <property name="default-width">600</property>

                <property name="default-height">400</property>

                <property name="modal">true</property>

                <property name="deletable">false</property>

                <property name="destroy-with-parent">false</property>

                <property name="hide-on-close">true</property>

                <child>
                    <object class="GtkBox" id="gtk_boxDiag">
                        <property name="orientation">vertical</property>

                        <child>
                            <object class="AdwHeaderBar" id="adw_headerBar">
                                <property name="show-end-title-buttons">true</property>

                                <property name="title-widget">
                                    <object class="AdwWindowTitle">
                                        <property name="title" translatable="yes">Preferences</property>
                                    </object>
                                </property>

                                <child type="end">
                                    <object class="GtkButton" id="gtk_btnSave">
                                        <style>
                                            <class name="suggested-action"/>
                                        </style>

                                        <property name="label" translatable="yes">Save</property>
                                    </object>
                                </child>
                            </object>
                        </child>

                        <child>
                            <object class="AdwPreferencesPage" id="adw_prefPage">
                                <child>
                                    <object class="AdwPreferencesGroup" id="adw_groupUserInterface">
                                        <property name="title" translatable="yes">User Interface</property>

                                        <property name="description" translatable="yes">Customize the application's user interface.</property>

                                        <child>
                                            <object class="AdwComboRow" id="adw_rowTheme">
                                                <property name="title" translatable="yes">Theme</property>

                                                <property name="model">
                                                    <object class="GtkStringList">
                                                      <items>
                                                        <item translatable="yes">System</item>
                                                        <item translatable="yes">Light</item>
                                                        <item translatable="yes">Dark</item>
                                                      </items>
                                                    </object>
                                                </property>
                                            </object>
                                        </child>
                                    </object>
                                </child>

                                <child>
                                    <object class="AdwPreferencesGroup" id="adw_groupApplication">
                                        <property name="title" translatable="yes">Application</property>

                                        <property name="description" translatable="yes">Customize application settings.</property>

                                        <child>
                                            <object class="AdwActionRow" id="adw_rowIsFirstTimeOpen">
                                                <property name="title" translatable="yes">Is First Time Open</property>

                                                <child>
                                                    <object class="GtkSwitch" id="gtk_switchIsFirstTimeOpen">
                                                        <property name="valign">center</property>

                                                        <property name="active">true</property>
                                                    </object>
                                                </child>
                                            </object>
                                        </child>
                                    </object>
                                </child>
                            </object>
                        </child>
                    </object>
                </child>
            </object>
        </interface>
    )";
}
