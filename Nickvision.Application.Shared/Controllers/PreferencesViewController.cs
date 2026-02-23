using Nickvision.Application.Shared.Models;
using Nickvision.Desktop.Application;
using Nickvision.Desktop.Filesystem;
using Nickvision.Desktop.Globalization;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nickvision.Application.Shared.Controllers;

public class PreferencesViewController
{
    private readonly IJsonFileService _jsonFileService;
    private readonly ITranslationService _translationService;
    private readonly Configuration _configuration;

    public IReadOnlyList<SelectionItem<string>> AvailableTranslationLanguages { get; }
    public IReadOnlyList<SelectionItem<Theme>> Themes { get; }

    public PreferencesViewController(IJsonFileService jsonFileService, ITranslationService translationService)
    {
        _jsonFileService = jsonFileService;
        _translationService = translationService;
        _configuration = _jsonFileService.Load<Configuration>(Configuration.Key);
        AvailableTranslationLanguages = new List<SelectionItem<string>>()
        {
            new SelectionItem<string>(string.Empty, _translationService._("System"), string.IsNullOrEmpty(_configuration.TranslationLanguage)),
            new SelectionItem<string>("C", "en_US", _configuration.TranslationLanguage == "C")
        };
        var languages = _translationService.AvailableLanguages.ToList();
        languages.Sort();
        foreach (var language in languages)
        {
            (AvailableTranslationLanguages as IList)!.Add(new SelectionItem<string>(language, language, _configuration.TranslationLanguage == language));
        }
        Themes = new List<SelectionItem<Theme>>()
        {
            new SelectionItem<Theme>(Models.Theme.Light, _translationService._p("Theme", "Light"), _configuration.Theme == Models.Theme.Light),
            new SelectionItem<Theme>(Models.Theme.Dark, _translationService._p("Theme", "Dark"), _configuration.Theme == Models.Theme.Dark),
            new SelectionItem<Theme>(Models.Theme.System, _translationService._p("Theme", "System"), _configuration.Theme == Models.Theme.System),
        };
    }
    public SelectionItem<Theme> Theme
    {
        set => _configuration.Theme = value.Value;
    }

    public SelectionItem<string> TranslationLanguage
    {
        set => _configuration.TranslationLanguage = value.Value;
    }

    public bool AllowPreviewUpdates
    {
        get => _configuration.AllowPreviewUpdates;

        set => _configuration.AllowPreviewUpdates = value;
    }

    public async Task SaveConfigurationAsync() => await _jsonFileService.SaveAsync(_configuration, Configuration.Key);
}
