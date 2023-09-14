const string appId = "org.nickvision.application";
const string projectName = "NickvisionApplication";
const string shortName = "application";
readonly string[] projectsToBuild = new string[] { "GNOME" };

if (FileExists("CakeScripts/main.cake"))
{
    #load local:?path=CakeScripts/main.cake
}
else
{
    throw new CakeException("Failed to load main script.");
}